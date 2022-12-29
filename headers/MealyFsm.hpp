#ifndef MEALY_FSM_HPP 
#define MEALY_FSM_HPP

#include "Fsm.hpp"
#include "State.hpp"
#include "MooreFsm.hpp"

class MealyFsm : public Fsm {
public:
    using Transition = Fsm::Transition;
    using Table = std::vector<std::vector<Transition>>;

    MealyFsm();
    MealyFsm(size_t stateQty, size_t entrySignalQty);

    void AddRule(State sourceState, 
                Signal entrySignal, 
                State destState,
                Signal exitSignal);
    void Read(std::istream& input) override;
    void Print(std::ostream& output) const override;

    unsigned GetStateQuantity() const override;
    unsigned GetEntrySignalQuantity() const override;
    Fsm::Transition GetTransition(State, Signal) const override;

private:
    void SetIdentifiers();
    void Resize(size_t stateQty, size_t entrySignalQty);

    Table m_table;
};

#endif // MEALY_FSM_HPP