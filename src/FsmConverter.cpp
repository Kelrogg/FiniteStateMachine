#include "pch.hpp"

#include "FsmConverter.hpp"

MooreFsm FsmConverter::ConvertToMoore(Fsm const& machine){
    std::set<Fsm::Transition> alphabet;

    // Generate new states for Moore
    for (unsigned state = 0; state < machine.GetStateQuantity(); ++state)
        for (unsigned signal = 0; signal < machine.GetEntrySignalQuantity(); ++signal)
            alphabet.insert(machine.GetTransition(state, signal));

    MooreFsm moore(alphabet.size(), machine.GetEntrySignalQuantity());
    
    for (auto it = alphabet.cbegin(); it != alphabet.cend(); ++it) {
        auto const& [mealyState, _] = *it;
        State mooreSourceState = std::distance(alphabet.cbegin(), it); // TODO unordered_map[] instead set.find() (std::hash for Transition)

        for (unsigned signal = 0; signal < machine.GetEntrySignalQuantity(); ++signal) {
            State destState = std::distance(alphabet.cbegin(),
                alphabet.find(machine.GetTransition(mealyState, signal))
            );
            moore.AddRule(mooreSourceState, signal, destState);
        }
    }

    for (auto it = alphabet.cbegin(); it != alphabet.cend(); ++it) {
        moore.RebindExitSignal(std::distance(alphabet.cbegin(), it), it->second);
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