#include <iostream>
#include "Dorian/Dorian.hpp"
#include "main.hpp"

using namespace Dorian;

Dorian::Window GameWindow;

#ifndef DORIAN_INITSCENE
#define DORIAN_INITSCENE TestScene

int TestInit() {return 0;}
int TestDraw(int a) {return 0;}
int TestUpdate(int a) {return 0;}

LLScene TestScene(TestInit, TestDraw, TestUpdate);
#endif

int main() {
    GameWindow = Window("Mariana's Trench");
    // Running the Engine
    GameWindow.Init(&DORIAN_INITSCENE);
    InitAssets();

    GameWindow.Loop();

    UnloadAssets();

    return 0;
}