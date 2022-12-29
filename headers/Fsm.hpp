#ifndef FSM_HPP 
#define FSM_HPP

#include "State.hpp"

class Fsm {
public:
    using Transition = std::pair<State, Signal>;

    virtual void Read(std::istream& input) = 0;
    virtual void Print(std::ostream& output) const = 0;

    virtual unsigned GetStateQuantity() const = 0;
    virtual unsigned GetEntrySignalQuantity() const = 0;
    virtual Transition GetTransition(State sourceState, Signal entrySignal) const = 0;

    virtual ~Fsm() = default;

protected:
    std::string m_stateIdentifier;
    std::string m_exitSignalIdentifier;
};

#endif // FSM_HPP