#ifndef GAME_MAIN
#define GAME_MAIN

#include "Dorian/Dorian.hpp"

struct SubmarineSpecs {
    int Durability;
    float Oxygen;
    int Research;
    float Depth;
    float Speed;
};

extern ui128 Money;

extern SubmarineSpecs CurrrentSubStats;

extern Dorian::LLScene HomeScene;
extern Dorian::LLScene ExplorationScene;
extern Dorian::LLScene TitleScene;

extern Dorian::Window GameWindow;

// Assets

namespace Assets {
    extern Dorian::Image SubmarineImage;
    extern Dorian::Image Logo;
    extern Dorian::Image Background;

    extern Dorian::Soundbank Sounds;
    extern Dorian::Song MainSong;
}

extern void InitAssets();
extern void UnloadAssets();

extern Dorian::Animated<ui8> faderOpacity;
extern Dorian::Animated<float> ShipY;

#endif