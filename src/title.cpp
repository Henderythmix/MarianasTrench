#include "main.hpp"
#include <fmt/format.h>

using namespace Dorian;

Animated<ui8> faderOpacity(255, {{255, 0}, {0, 1}, {255, 2}});

Button PlayButton({10, 10}, {100, 25}, &GameWindow, BottomLeft);
UIImage UILogo(&Assets::Logo, {10, 10}, &GameWindow);
Label GameJam({10, 38}, "Made for Brackeys 2023.2", &GameWindow, BottomRight);
Label Credits({10, 10}, "Made by Henderythmix/Roberto Selles", &GameWindow, BottomRight);

int TitleSceneInit() {
    PlayMusicStream(Assets::MainSong);
    PlayButton.Text = "Play";
    faderOpacity.Play({0, 1});
    UILogo.resize(3);
    return 0;
}

int TitleSceneUpdate(int) {
    UpdateMusicStream(Assets::MainSong);

    faderOpacity.Update(GameWindow.getDT());
    if (PlayButton.Update()) {
        PlaySound(Assets::Sounds[0]);
        GameWindow.LoadScene(&HomeScene);
    }

    return 0;
}

int TitleSceneDraw(int) {
    Assets::Background.Draw({0, 0}, 3);
    PlayButton.Draw();
    UILogo.Draw();
    GameJam.Draw();
    Credits.Draw();

    // The Fader
    DrawRect({0, 0}, GameWindow.GetWindowDimensions(), {0, 0, 0, faderOpacity.get()});

    //DrawText(fmt::format("{}, {}, {}", GetMusicTimePlayed(Assets::MainSong), IsMusicStreamPlaying(Assets::MainSong), IsMusicReady(Assets::MainSong)), {10, 10});

    return 0;
}

LLScene TitleScene(TitleSceneInit, TitleSceneDraw, TitleSceneUpdate);