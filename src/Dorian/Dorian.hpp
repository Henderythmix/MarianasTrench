#ifndef DORIAN_BASE
#define DORIAN_BASE

#include "Math.hpp"
#include <string>
#include <vector>
#include <raylib.h> // Dang it... I didn't want to include this in here :/

#ifndef DWINDOW_WIDTH
#define DWINDOW_WIDTH 1080
#endif

#ifndef DWINDOW_HEIGHT
#define DWINDOW_HEIGHT 720
#endif

#define DORIAN_IMAGETYPE Texture2D

namespace Dorian {
    // Scene Class

    // Base Scene Class used as a base for LLScene and HLScene.
    // Should not be used
    class Scene {
        public:
        virtual void Init() {};
        virtual void Update() {};
        virtual void Draw() {};
    };

    // A Low Level Scene for Low Level Programming
    // Used for Manually Programming your own Loop Functions
    // No nodes are used in this components. if you would like to use those, use HLScene
    class LLScene: public Scene {
        private:
        int (*M_InitPF)();
        int (*M_DrawPF)(int);
        int (*M_UpdatePF)(int);

        int m_InitState{0};

        public:
        LLScene(int initfunc (), int drawfunc (int), int updatefunc (int));

        virtual void Init() { m_InitState = (*M_InitPF)(); };
        virtual void Update() { (*M_UpdatePF)(m_InitState); };
        virtual void Draw() { (*M_DrawPF)(m_InitState); };
    };


    // A High Level Scene for Multiple Objects in a scene
    // Add nodes to your Scene and have them all run together
    // If you need to go more complex, I would suggest LLScene
    // class HLScene: public Scene {
    //     public:
    //     std::vector<bool> UIComponents;
    //     std::vector<Node> WorldComponents;
    // };

    enum Anchor {TopLeft, Top, TopRight, Left, Center, Right, BottomLeft, Bottom, BottomRight};

    // The Default Window Class of the Dorian Engine
    class Window {
        private:
        Scene* m_CurrentScene;
        float m_CurrentDT;

        std::string m_WindowName;
        i32 m_windowWidth;
        i32 m_windowHeight;

        public:
        // Constructor
        Window(std::string title, i32 width, i32 height);
        Window(std::string title): Window(title, DWINDOW_WIDTH, DWINDOW_HEIGHT) {};

        // The Generic Window Class Constructor
        Window(): Window("Dorian Engine", DWINDOW_WIDTH, DWINDOW_HEIGHT) {};
        
        float getDT();
        // Load a Scene into the game
        void LoadScene(Scene*);
        // Call this to officially initialize the game
        void Init(Scene*);
        // The primary loop function of a Dorian Window
        void Loop();

        Math::Vec2<i32> GetWindowDimensions() {return {m_windowWidth, m_windowHeight};};

        // TODO: Get the rest of the anchors programmed in the game.
        // They are not needed for this game specifically, but could help in the future
        Math::Vec2<int> AnchorOnScreen(Math::Vec2<int> padding, Math::Vec2<int> size, Anchor anchor) {
            // Please fill this in properly
            switch (anchor) {
                case TopRight:
                    return {m_windowWidth - padding.X - size.X, padding.Y};
                case BottomLeft:
                    return {padding.X, m_windowHeight - padding.Y - size.Y};
                case BottomRight:
                    return {m_windowWidth - padding.X - size.X, m_windowHeight - padding.Y - size.Y};
            }

            // Default Case TopLeft
            return padding;
        }
    };

    //----------------//
    // Basic Graphics //
    //----------------//

    struct RGB {
        ui8 R;
        ui8 G;
        ui8 B;
    };

    struct RGBA {
        ui8 R;
        ui8 G;
        ui8 B;
        ui8 A;
    };

    extern void DrawSprite();

    extern void DrawRect(Math::Vec2<int> pos, Math::Vec2<int> size);
    extern void DrawRect(Math::Vec2<int> pos, Math::Vec2<int> size, RGB rgb);
    extern void DrawRect(Math::Vec2<int> pos, Math::Vec2<int> size, RGBA rgba);

    extern void DrawText(std::string text, Math::Vec2<int> pos);
    extern void DrawText(std::string text, Math::Vec2<int> pos, RGB rgb);
    
    extern void FillBG(RGB rgb);
    extern void FillBG(RGBA rgba);

    class Image {
        private:
        DORIAN_IMAGETYPE m_imgData;

        public:
        Image() {};
        Image(std::string src);
        //~Image();

        void Draw(Math::Vec2<int> screenPos);
        void Draw(Math::Vec2<int> screenPos, float Scale);

        // Draws the texture to the center of the point specified
        void DrawCenter(Math::Vec2<int> screenPos);
        void DrawCenter(Math::Vec2<int> screenPos, float Scale);

        Math::Vec2<int> GetDimensions() {return {m_imgData.width, m_imgData.height};};
    };

    //-----------//
    // UI MODULE //
    //-----------//

    class UIComponent {
        protected:
        Anchor m_AnchorTo;
        Math::Vec2<int> m_Pos;
        Math::Vec2<int> m_Size;
        Window* m_WindowP;

        public:
        UIComponent(Math::Vec2<int> pos, Math::Vec2<int> size, Window* window, Anchor a) {
            m_WindowP = window;
            m_AnchorTo = a;
            m_Size = size;
            m_Pos = pos;
        };

        virtual void Draw() {};
        virtual bool Update() {return false;};
    };

    class Label: public UIComponent {
        public:
        std::string Text{""};

        Label(Math::Vec2<int> pos, std::string text, Window* window, Anchor a): UIComponent(pos, Math::Vec2<int>{(int)text.length()*10, 18}, window, a) {
            Text = text;
        };

        Label(Math::Vec2<int> pos, std::string text, Window* window): Label(pos, text, window, TopLeft) {};
        
        virtual void Draw() {
            DrawText(Text, m_WindowP->AnchorOnScreen(m_Pos, m_Size, m_AnchorTo));
        }
    };

    class UIImage: public UIComponent {
        private:
        Image* m_Image;
        float m_scale;
        
        public:

        UIImage(Image* img, Math::Vec2<int> pos, Window* window): UIComponent(pos, {1, 1}, window, TopLeft) {
            m_Image = img;
            m_Size = m_Image->GetDimensions();
            m_scale = 1;
        }

        void resize(int s) {
            m_scale = s;
        }

        virtual void Draw() {
            if (m_AnchorTo == TopLeft)
                m_Image->Draw(m_Pos, m_scale);
        }
    };

    class Button: public UIComponent {
        private:
        RGB m_ButtonBGColor{255, 255, 255};

        public:
        std::string Text{""};

        Button(Math::Vec2<int> pos, Math::Vec2<int> size, Window* window, Anchor a): UIComponent(pos, size, window, a) {};
        Button(Math::Vec2<int> pos, Math::Vec2<int> size, Window* window): UIComponent(pos, size, window, TopLeft) {};

        virtual void Draw() {
            Math::Vec2<int> textpos{m_WindowP->AnchorOnScreen(m_Pos, m_Size, m_AnchorTo)};
            DrawRect(m_WindowP->AnchorOnScreen(m_Pos, m_Size, m_AnchorTo), m_Size, m_ButtonBGColor);
            DrawText(Text, {textpos.X + m_Size.X/2 - (int)(Text.size()*9/2), textpos.Y + m_Size.Y/2 - 9}, {0, 0, 0});
        };
        virtual bool Update();
    };

    // Animations

    template <class T>
    struct AnimationFrame {
        T value;
        float time;
    };

    template <class T>
    class Animated {
        private:
        T m_CurrentValue;
        std::vector<AnimationFrame<T>> m_AnimationFrames;
        Math::Vec2<int> m_currentFrameIndex;
        bool m_Playing{false};

        public:
        float Time{0};
        bool Looping;

        Animated(T initialVal, std::vector<AnimationFrame<T>> frames, bool looping = false) {
            m_CurrentValue = initialVal;
            m_AnimationFrames = frames;
            m_currentFrameIndex = {0, (int)frames.size() - 1};
            Looping = looping;
        };

        void Play() {
            Time = 0;
            m_Playing = true;
        };

        void Play(Math::Vec2<int> range) {
            Time = m_AnimationFrames[range.X].time;
            m_currentFrameIndex = range;
            m_Playing = true;
        };

        T get() {return m_CurrentValue;};
        void Update(float t) {
            if (!m_Playing) return;
            Time += t;
            
            if (Time >= m_AnimationFrames[m_currentFrameIndex.Y].time && Looping) {
                Time -= m_AnimationFrames[m_currentFrameIndex.Y].time - m_AnimationFrames[m_currentFrameIndex.X].time;
            }

            for (int i{m_currentFrameIndex.X}; i < m_currentFrameIndex.Y; i++) {
                if (Time >= m_AnimationFrames[i].time && Time < m_AnimationFrames[i+1].time) {
                    m_CurrentValue = (m_AnimationFrames[i+1].value-m_AnimationFrames[i].value)/(m_AnimationFrames[i+1].time-m_AnimationFrames[i].time)
                    *(Time - m_AnimationFrames[i].time) + m_AnimationFrames[i].value;
                    break;
                }
            }
        }
    };

    // Some Sample Animations

    // Audio

    // At this point, I can tell I am just fed up with the game jam,
    // so I am just doing this at this point
    typedef std::vector<Sound> Soundbank;
    typedef Music Song;
}

#endif