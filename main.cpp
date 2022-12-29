#include "pch.hpp"

#include "Fsm.hpp"
#include "MealyFsm.hpp"
#include "MooreFsm.hpp"
#include "FsmConverter.hpp"

int main(int, char**) {
    //MooreFsm machine;
    MooreFsm machine;
    std::ifstream in{"input1.txt"};
    std::ofstream out{"output.txt"};
    machine.Read(in);
    FsmConverter b;
    MealyFsm a = b.ConvertToMealy(machine);
    a.Print(out);
}
