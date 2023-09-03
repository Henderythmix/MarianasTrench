#include "main.hpp"
#include <cmath>
#include <math.h>
#include <fmt/format.h>

using namespace Dorian;

template<class T>
ui128 GetPrice(T stat, T amount) {return pow(amount, stat/amount/3.14);};

template<class T>
int GetLevel(T stat, T amount) {return stat/amount; };

template<class T>
bool UpgradeStat(T& stat, T amount) {
    ui128 Price = pow(amount, stat/amount/3.14);

    if (Money >= Price) {
        PlaySound(Assets::Sounds[0]);
        stat += amount;
        Money -= Price;
        return true;
    }
    
    PlaySound(Assets::Sounds[1]);

    return false;
}

SubmarineSpecs CurrrentSubStats{1000, 100, 0, 0, 1000};

// The UI

Button ExploreButton({10, 10}, {100, 25}, &GameWindow, BottomLeft);
Button UpgradeDurability({10, 10}, {300, 25}, &GameWindow, TopRight);
Button UpgradeOxygen({10, 45}, {300, 25}, &GameWindow, TopRight);
Button UpgradeSpeed({10, 80}, {300, 25}, &GameWindow, TopRight);

Label MoneyLabel({10, 10}, "Money: 0", &GameWindow);

// Scene Functions

int HomeSceneInit() {
    PlayMusicStream(Assets::MainSong);
    faderOpacity.Play({0, 1});
    ExploreButton.Text = "Explore";
    return 0;
}

int HomeSceneDraw(int) {

    // Drawing the Scene
    Assets::Background.Draw({0, 0}, 3);
    Assets::SubmarineImage.DrawCenter(Math::ScaleVec<int>(GameWindow.GetWindowDimensions(), 0.5f), 3);

    // Drawing UI
    MoneyLabel.Text = fmt::format("Money: ${}", Money);
    MoneyLabel.Draw();
    ExploreButton.Draw();

    UpgradeDurability.Text = fmt::format("Resistance {}: ${}", 
        GetLevel<int>(CurrrentSubStats.Durability, 1000), GetPrice<int>(CurrrentSubStats.Durability, 1000));
    UpgradeDurability.Draw();

    UpgradeOxygen.Text = fmt::format("Oxygen Tank {}: ${}", 
        GetLevel<float>(CurrrentSubStats.Oxygen, 100), GetPrice<float>(CurrrentSubStats.Oxygen, 100));
    UpgradeOxygen.Draw();

    UpgradeSpeed.Text = fmt::format("Speed {}: ${}", 
        GetLevel<float>(CurrrentSubStats.Speed, 1000), GetPrice<float>(CurrrentSubStats.Speed, 1000));
    UpgradeSpeed.Draw();

    DrawRect({0, 0}, GameWindow.GetWindowDimensions(), {0, 0, 0, faderOpacity.get()});

    return 0;
}

int HomeSceneUpdate(int) {
    // Animations
    faderOpacity.Update(GameWindow.getDT());
    
    UpdateMusicStream(Assets::MainSong);

    // The UI
    if (ExploreButton.Update()) {
        PlaySound(Assets::Sounds[0]);
        StopMusicStream(Assets::MainSong);
        GameWindow.LoadScene(&ExplorationScene);
    }
    if (UpgradeDurability.Update()) UpgradeStat<int>(CurrrentSubStats.Durability, 1000);
    if (UpgradeOxygen.Update()) UpgradeStat<float>(CurrrentSubStats.Oxygen, 100);
    if (UpgradeSpeed.Update() && CurrrentSubStats.Speed/1000 <= 20) UpgradeStat<float>(CurrrentSubStats.Speed, 1000);

    return 0;
}

#ifndef TESTING
Dorian::LLScene HomeScene(HomeSceneInit, HomeSceneDraw, HomeSceneUpdate);
#endif

//---------//
// TESTING //
//---------//

#ifdef TESTING
#include <iostream>

ui128 Money{10000000000};

void printStats() {
    std::cout << "Durability: " << CurrrentSubStats.Durability << std::endl;
    std::cout << "Oxygen Tank: " << CurrrentSubStats.Oxygen << std::endl;
    std::cout << "Speed: " << CurrrentSubStats.Speed << std::endl;
    std::cout << "Money: " << Money << std:: endl;
}

int main() {
    printStats();
    UpgradeStat<int>(CurrrentSubStats.Durability, 1000);
    printStats();
    UpgradeStat<int>(CurrrentSubStats.Durability, 1000);
    printStats();
    UpgradeStat<int>(CurrrentSubStats.Durability, 1000);
    printStats();
    return 0;
}
#endif