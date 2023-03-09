#include "pch.hpp"

#include "utility.hpp"
#include "MealyFsm.hpp"

MealyFsm::MealyFsm()
{
    SetIdentifiers();
}

MealyFsm::MealyFsm(size_t stateQty, size_t entrySignalQty)
{
    Resize(stateQty, entrySignalQty);
    SetIdentifiers();
};

std::vector<MealyFsm::StateClass> MealyFsm::InitialSplit() const {
    return std::vector<MealyFsm::StateClass>();
};

void MealyFsm::SweepUnreachableStates() {
    if (GetStateQuantity() == 0) return;

    std::vector<unsigned> offset;
    offset.reserve(GetStateQuantity());

    std::vector<bool> reachableState(GetStateQuantity());
    *reachableState.begin() = true;

    std::queue<State> q;
    q.push(0);
    unsigned newStateQuantity = 1;

    // find unreachable
    while (!q.empty()) {
        State currentState = q.front(); q.pop();
        for (unsigned entrySignal = 0; entrySignal < GetEntrySignalQuantity(); ++entrySignal) 
        {
            auto const& [state, signal] = GetTransition(currentState, entrySignal);
            if (state.IsOk() && !reachableState[state.Index()]) {
                reachableState[state.Index()] = true;
                q.push(state);
                ++newStateQuantity;
            }
        }
    }

    // fast erase unreachable
    Table newTable;
    newTable.reserve(newStateQuantity);

    unsigned stateIndex = 0;
    for (auto const& inTable : reachableState) {
        if (inTable) {
            newTable.push_back(std::move(m_table[stateIndex]));
        }
        ++stateIndex;
    }
    m_table = std::move(newTable);

    // recalculate table states
    unsigned int stateOffset = 0;
    for (auto const& isReachable : reachableState) {
        offset.push_back(stateOffset);
        if (!isReachable) 
            ++stateOffset;
    }
    for (auto& row : m_table)
        for (unsigned i = 0; i < row.size(); ++i) {
            auto& [state, _] = row[i];
            if (state.IsOk())
                state -= offset[state.Index()];
        }
}

inline size_t ReadIdentifier(std::string const& input, std::string& line, size_t pos = 0)
{
    auto beg = input.begin();
    std::advance(beg, pos);
    
    beg = std::find_if(beg, input.end(), isalpha);
    auto end = std::find_if_not(beg, input.end(), isalpha);
    
    line = std::string(beg, end);
    return std::distance(input.begin(), end);
}

inline size_t ReadTransition(std::string const& input,
                            MealyFsm::Transition& transition, size_t pos = 0)
{
    auto& [state, signal] = transition;
    auto beg = input.begin();
    std::advance(beg, pos);

    auto failState = std::find(beg, input.cend(), State::failStateSymbol);

    beg = std::find_if(beg, input.end(), isdigit);
    if (beg > failState) {
        state.SetFail();
        return std::distance(input.begin(), std::next(failState));
    }
    auto end = std::find_if_not(beg, input.end(), isdigit);
    state = stounsigned(beg, end);

    beg = std::find_if(end, input.end(), isdigit);
    end = std::find_if_not(beg, input.end(), isdigit);
    signal = stounsigned(beg, end);

    return std::distance(input.begin(), end);
}

void MealyFsm::Read(std::istream& input) {
    std::string line;
    std::getline(input, line);
    auto lineStream = std::istringstream(line);

    size_t stateQty, entrySignalQty;
    lineStream >> stateQty;
    if (lineStream.fail())
    {
        throw std::invalid_argument("Failed to read state`s quantity");
    }
    lineStream >> entrySignalQty;
    if (lineStream.fail())
    {
        throw std::invalid_argument("Failed to read signal`s quantity");
    }

    Resize(stateQty, entrySignalQty);

    std::getline(input, line);

    size_t pos = ReadIdentifier(line, m_stateIdentifier);
    ReadIdentifier(line, m_exitSignalIdentifier, pos);

    // TODO
    pos = 0;
    auto stateRow = m_table.begin();
    auto cell = stateRow->begin();
    for (; cell < stateRow->end(); ++cell){
        pos = ReadTransition(line, *cell, pos);
    }
    ++stateRow;

    for (; stateRow < m_table.end() && std::getline(input, line); 
            ++stateRow) 
    {
        cell = stateRow->begin();
        pos = 0;
        for (; cell < stateRow->end(); ++cell) {
            pos = ReadTransition(line, *cell, pos);
        }
    }
    //
    SweepUnreachableStates();
}

void MealyFsm::Print(std::ostream& output) const {
    for (auto stateRow = m_table.cbegin(); stateRow < m_table.cend(); ++stateRow) {
        for (auto cell = stateRow->cbegin(); cell < stateRow->cend(); ++cell) {
            auto& [destState, exitSignal] = *cell;
            if (destState.IsOk())
                output << m_stateIdentifier << destState << ' '
                        << m_exitSignalIdentifier << exitSignal << ' ';
            else 
                output << State::failStateSymbol << ' ';
        }
        output << '\n';
    }
}

void MealyFsm::Resize(size_t stateQty, size_t entrySignalQty) {
    m_table.resize(stateQty);
    for (auto& row : m_table) 
        row.resize(entrySignalQty);
}

void MealyFsm::SetIdentifiers() {
    m_exitSignalIdentifier = "Y";
    m_stateIdentifier = "S";    
}

inline unsigned MealyFsm::GetStateQuantity() const noexcept {
    return m_table.size();
}

inline unsigned MealyFsm::GetEntrySignalQuantity() const noexcept {
    return m_table.empty() ? 0 : m_table[0].size();
}

MealyFsm::Transition MealyFsm::GetTransition(State source, Signal entry) const {
    if (!source.IsOk() || 
        m_table.size() <= source.Index() || m_table[0].size() <= entry.Index()
    ) {
        throw std::out_of_range("Wrong index");
    }
    return m_table[source.Index()][entry.Index()];
}

void MealyFsm::AddRule(State source, Signal entrySignal, State dest, Signal exitSignal) {
    if (!source.IsOk()) {
        throw std::logic_error("Cannot make transition from empty state");
    }

    if (m_table.size() <= source.Index() || m_table[0].size() <= entrySignal.Index()) {
        throw std::out_of_range("Wrong index");
    }
    m_table[source.Index()][entrySignal.Index()] = {dest, exitSignal};
}