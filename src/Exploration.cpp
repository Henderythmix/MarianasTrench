#include "main.hpp"

#include <string>
#include <vector>
#include <cstdlib>
#include <fmt/format.h>

using namespace Dorian;

ui128 Money;

#define ColorChangeRate 255/11034

RGBA SeaColor{0x17, 0x2e, 0x8f, 255};
Animated<float> ShipY(0, {{0, 0}, {10, 1}, {-10, 3}, {0, 4}, {1080, 14}}, true);

// Useful Classes

class Submarine {
    protected:
    SubmarineSpecs m_Stats;

    public:
    Submarine() {};
    Submarine(SubmarineSpecs SubData) {
        m_Stats = SubData;

        // Yes... I agree... This is dumb...
        // I needed to multiply this stat to make it more valuable :/
        m_Stats.Speed /= 1000;
    };

    // Pulling the variables out
    int getResistance() {return m_Stats.Durability;};
    float getOxygen() {return m_Stats.Oxygen;};
    int getResearch() {return m_Stats.Research;};
    float getDepth() {return m_Stats.Depth;};

    void DrainOxygen(float a) {m_Stats.Oxygen -= a;};
    void TakeDamage(unsigned a) {m_Stats.Durability -= a;};
    void Discover(unsigned a) {m_Stats.Research += a;};

    // See Documentation/Game/Submarine.md
    void AddVelocityRelativeToDistance() {
        float s = m_Stats.Speed/m_Stats.Durability;
        m_Stats.Depth += s*(m_Stats.Durability-m_Stats.Depth);
    }
};

struct ScenarioOption {
    std::string Option;
    int (*callback)();
};

class Scenario {
    private:
    std::string m_Description;
    std::vector<ScenarioOption> m_options;

    public:
    Scenario(std::string Desc) {
        m_Description = Desc;
    };

    std::string getDescription() {return m_Description;};

    void addOption(ScenarioOption AddedOption) {
        m_options.push_back(AddedOption);
    };
    void CallAction(int i) {
        (m_options[i].callback)();
    };
    std::string getOption(int i) {return m_options[i].Option;};
    int getOptionCount() {return m_options.size();};
};

// Initializing Variables

std::vector<Scenario> ScenarioList;
Submarine Sub;
std::string TextboxText{""};
int textRenderDistance{0};

//----------------//
// GAME FUNCTIONS //
//----------------//

void Textbox(std::string str);

#include "ScenarioCallbacks.inl"

//-----------------//
// SCENE FUNCTIONS //
//-----------------//

// UI Components
Label DepthLabel({10, 10}, "Depth: 0m", &GameWindow);
Label OxygenLeft({10, 38}, "Oxygen: 100.0L", &GameWindow);
Label EventDescription({10, 10}, "You are travelling deep into the ocean.", &GameWindow, BottomLeft);

Button ExitButton({10, 10}, {75, 25}, &GameWindow, TopRight);
Button OptionButton1({10, 10}, {100, 25}, &GameWindow, BottomRight);
Button OptionButton2({10, 45}, {100, 25}, &GameWindow, BottomRight);
Button OptionButton3({10, 80}, {100, 25}, &GameWindow, BottomRight);
Button OptionButton4({10, 115}, {100, 25}, &GameWindow, BottomRight);

#ifndef TESTING
void Textbox(std::string str) {
    EventDescription.Text = str;
}
#endif

bool Travelling{true};

int ExplorationSceneInit() {
    faderOpacity.Play({0, 1});
    ShipY.Looping = true;
    ShipY.Play({0, 3});
    
    Travelling = true;
    Textbox("You are travelling deep into the ocean.");
    Sub = Submarine(CurrrentSubStats);
    ExitButton.Text = "Return";

    // I hate this. I should probably move these to lua scripts in the future :/
    // Is it hard coded? yes. I kinda have too.
    if (ScenarioList.size() != 0) return 0;
    ScenarioList.push_back(Scenario("You encounter a shark."));
    ScenarioList[0].addOption({"Analyze", ResearchShark});
    ScenarioList[0].addOption({"Continue", Continue});

    ScenarioList.push_back(Scenario("You encounter a whale."));
    ScenarioList[1].addOption({"Analyze", ResearchWhale});
    ScenarioList[1].addOption({"Continue", Continue});

    ScenarioList.push_back(Scenario("You encounter an octopus."));
    ScenarioList[2].addOption({"Analyze", ResearchOctopus});
    ScenarioList[2].addOption({"Continue", Continue});

    ScenarioList.push_back(Scenario("You encounter a squid."));
    ScenarioList[3].addOption({"Analyze", ResearchSquid});
    ScenarioList[3].addOption({"Continue", Continue});

    ScenarioList.push_back(Scenario("You observe a speedy shark."));
    ScenarioList[4].addOption({"Analyze", ResearchAggresiveShark});
    ScenarioList[4].addOption({"Continue", Continue});

    ScenarioList.push_back(Scenario("You encounter a shark."));
    ScenarioList[5].addOption({"Analyze", ResearchShark});
    ScenarioList[5].addOption({"Pet", PetShark});
    ScenarioList[5].addOption({"Continue", Continue});

    ScenarioList.push_back(Scenario("A school of fish approach perpendicular to your submarine"));
    ScenarioList[6].addOption({"Wait", WaitFish});
    ScenarioList[6].addOption({"Continue", ScareFish});

    return 0;
}

int CurrentScenario{-1};
void CleanOptions() {
    OptionButton1.Text = "";
    OptionButton2.Text = "";
    OptionButton3.Text = "";
    OptionButton4.Text = "";
    CurrentScenario = -1;
}

int ExplorationSceneUpdate(int) {
    // Managing Animations
    faderOpacity.Update(GameWindow.getDT());
    ShipY.Update(GameWindow.getDT());

    // Manage Encounters and Exploration Distance
    static float timeUntilNextIteration{0};
    if (Travelling) timeUntilNextIteration += GameWindow.getDT();

    if (Sub.getOxygen() <= 0 && Travelling) {
        Travelling = false;
        ShipY.Looping = false;
        ShipY.Play({3, 4});
        Textbox("Your submarine imploded due to a lack of oxygen");
    }

    if (Sub.getDepth() >= 11030 && Travelling) {
        Travelling = false;
        ShipY.Looping = false;
        Textbox("You have made it to the bottom. Thank you for playing until the end!");
    }

    if (timeUntilNextIteration > 1 && Travelling) {
        Sub.AddVelocityRelativeToDistance();
        Sub.DrainOxygen(0.5);

        // Generate a Random Scenario if the timing is right
        CurrentScenario = rand() % (ScenarioList.size()*10);
        if (CurrentScenario < ScenarioList.size()) {
            Travelling = false;
            Textbox(ScenarioList[CurrentScenario].getDescription());
            
            // PS: THIS IS NOT OPTIMAL
            // PLEASE FIX THIS EVENTUALLY
            if (ScenarioList[CurrentScenario].getOptionCount() > 0) OptionButton1.Text = ScenarioList[CurrentScenario].getOption(0);
            if (ScenarioList[CurrentScenario].getOptionCount() > 1) OptionButton2.Text = ScenarioList[CurrentScenario].getOption(1);
            if (ScenarioList[CurrentScenario].getOptionCount() > 2) OptionButton3.Text = ScenarioList[CurrentScenario].getOption(2);
            if (ScenarioList[CurrentScenario].getOptionCount() > 3) OptionButton4.Text = ScenarioList[CurrentScenario].getOption(3);

        }

        timeUntilNextIteration -= 1;
    }

    // Wait for Response

    // THIS IS NOT OPTIMAL EITHER
    // PLEASE FIX THIS TOO
    if (!Travelling && CurrentScenario != -1) {
        if (OptionButton1.Update() && ScenarioList[CurrentScenario].getOptionCount() >= 1) {
            ScenarioList[CurrentScenario].CallAction(0);
            Travelling = true;
            CleanOptions();
        }

        if (OptionButton2.Update() && ScenarioList[CurrentScenario].getOptionCount() >= 2) {
            ScenarioList[CurrentScenario].CallAction(1);
            Travelling = true;
            CleanOptions();
        }

        if (OptionButton3.Update() && ScenarioList[CurrentScenario].getOptionCount() >= 3) {
            ScenarioList[CurrentScenario].CallAction(2);
            Travelling = true;
            CleanOptions();
        }

        if (OptionButton4.Update() && ScenarioList[CurrentScenario].getOptionCount() >= 4) {
            ScenarioList[CurrentScenario].CallAction(3);
            Travelling = true;
            CleanOptions();
        }
    }

    // What if the traveller wanted to return to the surface?
    if (ExitButton.Update()) {
        PlaySound(Assets::Sounds[0]);
        if (Sub.getOxygen() > 0) Money += Sub.getDepth()*100 + Sub.getResearch()*500;
        GameWindow.LoadScene(&HomeScene);
        PlayMusicStream(Assets::MainSong);
    }

    return 0;
}

int ExplorationSceneDraw(int) {
    // Background
    //0x17, 0x2e, 0x8f

    SeaColor.R = 0x17*(11034-Sub.getDepth())/11034;
    SeaColor.G = 0x2e*(11034-Sub.getDepth())/11034;
    SeaColor.B = 0x8f*(11034-Sub.getDepth())/11034;

    FillBG(SeaColor);

    // Drawing the Scene
    Math::Vec2<int> ShipPos = Math::ScaleVec<int>(GameWindow.GetWindowDimensions(), 0.5f);
    ShipPos.Y += ShipY.get();
    Assets::SubmarineImage.DrawCenter(ShipPos, 3);
    
    // Drawing the UI
    DepthLabel.Text = fmt::format("Depth: {:g}m", Sub.getDepth());
    OxygenLeft.Text = fmt::format("Oxygen: {}L", Sub.getOxygen());

    DepthLabel.Draw();
    OxygenLeft.Draw();
    EventDescription.Draw();
    ExitButton.Draw();

    OptionButton1.Draw();
    OptionButton2.Draw();
    OptionButton3.Draw();
    OptionButton4.Draw();

    DrawRect({0, 0}, GameWindow.GetWindowDimensions(), {0, 0, 0, faderOpacity.get()});

    return 0;
}

// Although this object and class is defined in scenelist.hpp which includes Dorian/Dorian.hpp,  
// this preprocessor lets us `make test` for some reason.
// How do I remove the preprocessor? :/
#ifndef TESTING
LLScene ExplorationScene = LLScene(ExplorationSceneInit, ExplorationSceneDraw, ExplorationSceneUpdate);
#endif

//---------//
// TESTING //
//---------//

#ifdef TESTING

#include <iostream>

SubmarineSpecs CurrrentSubStats{1000, 100, 0, 0, 1};

void Textbox(std::string str) {
    std::cout << str << std::endl;
}

int main() {
    std::cout << "This is a Test Scenario for the main part of the game. This just lets you play as if this was a terminal text game." << std::endl;
    ExplorationSceneInit();

    std::cout << "Durability: " << Sub.getResistance() 
        << "\nOxygen: " << Sub.getOxygen()
        << "\nResearch: " << Sub.getResearch()
        << "\nDepth: " << Sub.getDepth() << std::endl;

    // 11033 and not 11034 because it would theoretically take an infinite amount of time to reach that distance in the way the formula is setup
    while (Sub.getDepth() < 11033) {
        Sub.AddVelocityRelativeToDistance();
        std::cout << Sub.getDepth() << std::endl;

        long long unsigned int i{std::rand() % (ScenarioList.size()*3)};
        if (i < ScenarioList.size()) {
            int userInput{0};
            Textbox(ScenarioList[i].getDescription());
            std::cout << "> ";
            std::cin >> userInput;
            std::cout << "You Chose to " << ScenarioList[i].getOption(userInput) << ": ";
            ScenarioList[i].CallAction(userInput);
        }

    }

    Textbox("You have made it to the bottom!");

    return 0;
}

#endif