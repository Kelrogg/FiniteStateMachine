#include "pch.hpp"

#include "FsmConverter.hpp"

MooreFsm FsmConverter::ConvertToMoore(Fsm const& machine){
    std::set<Fsm::Transition> alphabet;

    // Generate new states for Moore
    for (unsigned state = 0; state < machine.GetStateQuantity(); ++state)
        for (unsigned signal = 0; signal < machine.GetEntrySignalQuantity(); ++signal)
            {
                auto const& [fsmState, fsmSignal] = machine.GetTransition(state, signal);
                if (fsmState.IsOk())
                    alphabet.emplace(fsmState, fsmSignal);
            }

    MooreFsm moore(alphabet.size(), machine.GetEntrySignalQuantity());
    
    unsigned stateIndex = 0;
    for (auto const& [mealyState, _] : alphabet) {
        State mooreSourceState = stateIndex;

        for (unsigned signal = 0; signal < machine.GetEntrySignalQuantity(); ++signal) {
            auto const& [fsmState, fsmSignal] = machine.GetTransition(mealyState, signal);
            
            State destState;
            if (fsmState.IsOk())
                destState = std::distance(alphabet.cbegin(),
                    alphabet.find(machine.GetTransition(mealyState, signal)) // TODO unordered_map[] instead set.find() (std::hash for Transition)
                );
            else 
                destState.SetFail();

            moore.AddRule(mooreSourceState, signal, destState);
        }

        ++stateIndex;
    }

    stateIndex = 0;
    for (auto& [_, signal] : alphabet) {
        moore.RebindExitSignal(stateIndex, signal);
        ++stateIndex;
    }

    return moore;
}

MealyFsm FsmConverter::ConvertToMealy(Fsm const& machine) {
    MealyFsm mealy(machine.GetStateQuantity(), machine.GetEntrySignalQuantity());
    
    for (unsigned state = 0; state < machine.GetStateQuantity(); ++state) {
        for (unsigned signal = 0; signal < machine.GetEntrySignalQuantity(); ++signal) {
            auto const& [destState, bindSignal] = machine.GetTransition(state, signal);
            mealy.AddRule(state, signal, destState, bindSignal);
        }
    }

    return mealy;
}