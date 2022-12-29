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
    auto beg = input.begin();
    std::advance(beg, pos);

    beg = std::find_if(beg, input.end(), isdigit);
    auto end = std::find_if_not(beg, input.end(), isdigit);
    transition.first = stounsigned(beg, end);

    beg = std::find_if(end, input.end(), isdigit);
    end = std::find_if_not(beg, input.end(), isdigit);
    transition.second = stounsigned(beg, end);

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
}

void MealyFsm::Print(std::ostream& output) const {
    for (auto stateRow = m_table.cbegin(); stateRow < m_table.cend(); ++stateRow) {
        for (auto cell = stateRow->cbegin(); cell < stateRow->cend(); ++cell) {
            auto& [destState, exitSignal] = *cell;
            output << m_stateIdentifier << destState << ' '
                    << m_exitSignalIdentifier << exitSignal << ' ';
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

unsigned MealyFsm::GetStateQuantity() const {
    return m_table.size();
}

unsigned MealyFsm::GetEntrySignalQuantity() const {
    return m_table.empty() ? 0 : m_table[0].size();
}

MealyFsm::Transition MealyFsm::GetTransition(State source, Signal entry) const {
    if (m_table.size() <= source.Index() || m_table[0].size() <= entry.Index()) {
        throw std::out_of_range("Wrong index");
    }
    return m_table[source.Index()][entry.Index()];
}

void MealyFsm::AddRule(State sourceState, Signal entrySignal, State destState, Signal exitSignal) {
    if (m_table.size() <= sourceState.Index() || m_table[0].size() <= entrySignal.Index()) {
        throw std::out_of_range("Wrong index");
    }
    m_table[sourceState.Index()][entrySignal.Index()] = {destState, exitSignal};
}