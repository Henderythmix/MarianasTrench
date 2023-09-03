#include "main.hpp"

namespace Assets {
    Dorian::Image SubmarineImage;
    Dorian::Image Logo;
    Dorian::Image Background;

    Dorian::Soundbank Sounds;
    Dorian::Song MainSong;
}

void InitAssets() {
    Assets::SubmarineImage = Dorian::Image("./gfx/Submarine.png");
    Assets::Logo = Dorian::Image("./gfx/Logo.png");
    Assets::Background = Dorian::Image("./gfx/Upgrading_Wallpaper.png");

    Assets::Sounds.push_back(LoadSound("./sfx/MI_SFX 06.wav"));
    Assets::Sounds.push_back(LoadSound("./sfx/MI_SFX 04.wav"));

    Assets::MainSong = LoadMusicStream("./sfx/Song.wav");
}

void UnloadAssets() {
    UnloadSound(Assets::Sounds[0]);
    UnloadSound(Assets::Sounds[1]);
}