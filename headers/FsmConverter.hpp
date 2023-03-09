#ifndef MEALY_FSM_CONVERTER_HPP 
#define MEALY_FSM_CONVERTER_HPP

#include "Fsm.hpp"
#include "MealyFsm.hpp"
#include "MooreFsm.hpp"

class FsmConverter {
public:
    using MealyTransition = MealyFsm::Transition;
    using MealyTable = MealyFsm::Table; 

    using MooreExitSignals = MooreFsm::SignalBindings;
    using MooreTable = MooreFsm::Table;

    MooreFsm ConvertToMoore(Fsm const& machine);
    //MealyFsm ConvertToMealy(Fsm const& machine);
};

#endif