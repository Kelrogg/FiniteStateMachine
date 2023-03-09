#ifndef FSM_HPP 
#define FSM_HPP

#include "State.hpp"
#include "utility.hpp"

class Fsm {
public:
    using Transition = std::pair<State, Signal>;
    using StateClass = std::unordered_set<State, StateHasher>;

    virtual void SweepUnreachableStates() = 0;
    virtual void Minimize() = 0;
    virtual void Read(std::istream& input) = 0;
    virtual void Print(std::ostream& output) const = 0;

    virtual unsigned GetStateQuantity() const noexcept = 0;
    virtual unsigned GetEntrySignalQuantity() const noexcept = 0;
    virtual Transition GetTransition(State sourceState, Signal entrySignal) const = 0;

    virtual ~Fsm() = default;

protected:
    virtual std::vector<StateClass> InitialSplit() const = 0; // property 0_0

    std::string m_stateIdentifier;
    std::string m_exitSignalIdentifier;

private:
    std::tuple<StateClass, StateClass> Split(StateClass& initialClass, std::pair<StateClass*, Signal> const& splitter);
};

#endif // FSM_HPP