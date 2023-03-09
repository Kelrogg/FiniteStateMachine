#ifndef MOORE_FSM_HPP 
#define MOORE_FSM_HPP

#include "Fsm.hpp"
#include "State.hpp"
#include "MealyFsm.hpp"

class MooreFsm : public Fsm {
public:
    using Transition = Fsm::Transition;
    using Table = std::vector<std::vector<State>>;
    using SignalBindings = std::vector<Signal>;

    MooreFsm();
    MooreFsm(size_t stateQty, size_t entrySignalQty);

    void AddRule(State sourceState, 
                Signal entrySignal, 
                State destState);
    void RebindExitSignal(State state, Signal exitSignal);
    void SweepUnreachableStates() override;
    void Minimize() override;
    void Read(std::istream& input) override;
    void Print(std::ostream& output) const override;

    inline unsigned GetStateQuantity() const noexcept override;
    inline unsigned GetEntrySignalQuantity() const noexcept override;
    Fsm::Transition GetTransition(State, Signal) const override;
protected:
    std::vector<StateClass> InitialSplit() const override;
    
private:
    void SetIdentifiers();
    void Resize(size_t stateQty, size_t entrySignalQty);

    Table m_table;
    SignalBindings m_bindings;
};

#endif