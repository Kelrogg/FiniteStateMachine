#include "pch.hpp"

#include "Fsm.hpp"
#include "MealyFsm.hpp"
#include "MooreFsm.hpp"
#include "FsmConverter.hpp"

int main(int, char**) {
    MooreFsm machine;
    //MealyFsm machine;
    std::ifstream in{"input2.txt"};
    std::ofstream out{"output.txt"};
    machine.Read(in);
    FsmConverter b;
    MooreFsm a = b.ConvertToMoore(machine);
    //machine.Minimize();
    a.Print(out);
}
