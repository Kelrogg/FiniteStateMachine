#include "pch.hpp"

#include "utility.hpp"
#include "MooreFsm.hpp"
#include "MealyFsm.hpp"

MooreFsm::MooreFsm()
{
    SetIdentifiers();
}

MooreFsm::MooreFsm(size_t stateQty, size_t entrySignalQty)
{
    Resize(stateQty, entrySignalQty);
    SetIdentifiers();
};

void MooreFsm::SweepUnreachableStates() {
    
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

inline size_t ReadState(std::string const& input,
                            State& state, size_t pos = 0)
{
    auto beg = input.begin();
    std::advance(beg, pos);
    
    auto failState = std::find(beg, input.cend(), State::failStateSymbol);

    beg = std::find_if(beg, input.cend(), isdigit);
    if (beg > failState) {
        state.SetFail();
        return std::distance(input.begin(), std::next(failState));
    }

    auto end = std::find_if_not(beg, input.cend(), isdigit);
    state = stounsigned(beg, end);

    return std::distance(input.begin(), end);
}

inline size_t ReadSignal(std::string const& input,
                            Signal& exitSignal, size_t pos = 0)
{
    auto beg = input.begin();
    std::advance(beg, pos);

    beg = std::find_if(beg, input.cend(), isdigit);
    auto end = std::find_if_not(beg, input.cend(), isdigit);
    exitSignal = stounsigned(beg, end);

    return std::distance(input.begin(), end);
}

void MooreFsm::Read(std::istream& input) {
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

    //TODO
    do {
        std::getline(input, line);
    } while (line.find(State::failStateSymbol) == std::string::npos);

    size_t pos = ReadIdentifier(line, m_exitSignalIdentifier);
    ReadIdentifier(line, m_stateIdentifier, pos);

    pos = 0;
    auto stateRow = m_table.begin();
    auto cell = stateRow->begin();
    auto exitSignalBinding = m_bindings.begin();
    pos = ReadSignal(line, *exitSignalBinding, pos);
    for (; cell < stateRow->end(); ++cell) {
        pos = ReadState(line, *cell, pos);
    }
    ++stateRow;
    ++exitSignalBinding;

    for (; stateRow < m_table.end() && std::getline(input, line); 
            ++stateRow, ++exitSignalBinding) 
    {
        cell = stateRow->begin();
        pos = 0;
        pos = ReadSignal(line, *exitSignalBinding, pos);
        for (; cell < stateRow->end(); ++cell) {
            pos = ReadState(line, *cell, pos);
        }
    }
    //
}

void MooreFsm::Print(std::ostream& output) const {
    auto binding = m_bindings.cbegin();
    for (auto stateRow = m_table.cbegin(); stateRow < m_table.cend(); ++stateRow, ++binding) {
        output << m_exitSignalIdentifier << *binding << ' ';
        for (auto state = stateRow->cbegin(); state < stateRow->cend(); ++state) {
            if (state->IsOk())
                output << m_stateIdentifier << *state << ' ';
            else
                output << State::failStateSymbol << ' ';
        }
        output << '\n';
    }
}

void MooreFsm::Resize(size_t stateQty, size_t entrySignalQty)
{
    m_table.resize(stateQty);
    m_bindings.resize(stateQty);
    for (auto& row : m_table) 
        row.resize(entrySignalQty);
}

void MooreFsm::SetIdentifiers() {
    m_exitSignalIdentifier = "Y";
    m_stateIdentifier = "q";    
}

unsigned MooreFsm::GetStateQuantity() const {
    return m_table.size();
}

unsigned MooreFsm::GetEntrySignalQuantity() const {
    return m_table.empty() ? 0 : m_table[0].size();
}

MooreFsm::Transition MooreFsm::GetTransition(State source, Signal entry) const {
    if (!source.IsOk() ||
        m_table.size() <= source.Index() || m_table[0].size() <= entry.Index()
    ) {
        throw std::out_of_range("Wrong index");
    }

    State destState = m_table[source.Index()][entry.Index()];

    return {destState, m_bindings[destState.Index()]};
}

void MooreFsm::RebindExitSignal(State state, Signal exitSignal) {
    if (!state.IsOk()) {
        throw std::logic_error("Cannot bind exit signal to empty state");
    }

    if (m_bindings.size() <= state.Index()) {
        throw std::out_of_range("Wrong index");
    }
    m_bindings[state.Index()] = exitSignal;
}

void MooreFsm::AddRule(State source, Signal entrySignal, State dest) {
    if (!source.IsOk()) {
        throw std::logic_error("Cannot make transition from empty state");
    }

    if (m_table.size() <= source.Index() || m_table[0].size() <= entrySignal.Index()) {
        throw std::out_of_range("Wrong index");
    }
    m_table[source.Index()][entrySignal.Index()] = dest;
}