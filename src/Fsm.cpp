#include "pch.hpp"

#include "Fsm.hpp"

//  void Fsm::SweepUnreachableStates() {
//      if (GetStateQuantity() == 0) return;
//  
//      std::vector<bool> reachableState(GetStateQuantity());
//      *reachableState.begin() = true;
//  
//      std::queue<State> q;
//      q.push(0);
//      unsigned newStateQuantity = 1;
//  
//      // find unreachable
//      while (!q.empty()) {
//          State currentState = q.front(); q.pop();
//          for (unsigned entrySignal = 0; entrySignal < GetEntrySignalQuantity(); ++entrySignal) {
//              auto const& [state, signal] = GetTransition(currentState, entrySignal);
//              if (state.IsOk() && !reachableState[state.Index()]) {
//                  reachableState[state.Index()] = true;
//                  q.push(state);
//              }
//          }
//      }
//  
//  TODO effective for derived
//  }

std::tuple<Fsm::StateClass, Fsm::StateClass> 
Fsm::Split(StateClass& initialClass, std::pair<StateClass*, Signal> const& splitter) {
    StateClass R1;
    StateClass R2;

    auto& [ classSplitter, entrySignal] = splitter;
    for (auto& state : initialClass) {
        auto const& [destState, signal] = GetTransition(state, entrySignal);

        if (classSplitter->find(state) != classSplitter->cend())
            R1.emplace(state);
        else
            R2.emplace(state);
    }

    return std::make_tuple(std::move(R1), std::move(R2));
}

void Fsm::Minimize() {
    //std::vector<StateClass> equivalenceClasses;
    //equivalenceClasses.reserve(GetStateQuantity());
    //for (auto const& stateClass : InitialSplit())
    //    equivalenceClasses.emplace_back(stateClass);

    std::unordered_set<std::pair<StateClass*, Signal>, PairHasher<StateClass, Signal>> q;
 
    for (unsigned entrySignal = 0; entrySignal < GetEntrySignalQuantity(); ++entrySignal) {
        auto const split = InitialSplit();
        for (auto const& stateClass : split)
           q.insert(std::pair<StateClass*, Signal>(&stateClass, entrySignal));
    }

    //while (!q.empty()) {
    //    auto& splitter = *q.begin();

    //    for (auto& stateClass : equivalenceClasses) {
    //        auto const& [classR1, classR2] = Split(stateClass, splitter);
    //        if (!classR1.empty() && !classR2.empty()) {
    //            stateClass = classR1; // TODO
    //            equivalenceClasses.push_back(std::move(classR2));

    //            for (unsigned entrySignal = 0; entrySignal < GetEntrySignalQuantity(); ++entrySignal) {
    //                auto it = q.find({&stateClass, entrySignal});
    //                if (it != q.cend()) {
    //                    q.emplace(&*equivalenceClasses.cend());
    //                }
    //                else {
    //                    if (classR1.size() < classR2.size())
    //                        q.emplace(&stateClass);
    //                    else
    //                        q.emplace(&*equivalenceClasses.cend());
    //                }
    //            }
    //        }
    //    }

    //    q.erase(q.begin());
    //}

};
