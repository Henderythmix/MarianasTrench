#include "Dorian.hpp"
#include <raylib.h>

using namespace Dorian;

// Basic Graphics Drawing
void Dorian::DrawRect(Math::Vec2<int> pos, Math::Vec2<int> size) {
    DrawRectangle(pos.X, pos.Y, size.X, size.Y, WHITE);
}

void Dorian::DrawRect(Math::Vec2<int> pos, Math::Vec2<int> size, RGB rgb) {
    DrawRectangle(pos.X, pos.Y, size.X, size.Y, {rgb.R, rgb.G, rgb.B, 255});
}

void Dorian::DrawRect(Math::Vec2<int> pos, Math::Vec2<int> size, RGBA rgba) {
    DrawRectangle(pos.X, pos.Y, size.X, size.Y, {rgba.R, rgba.G, rgba.B, rgba.A});
}

void Dorian::DrawText(std::string text, Math::Vec2<int> pos) {
    ::DrawText(text.c_str(), pos.X, pos.Y, 18, WHITE);
}

void Dorian::DrawText(std::string text, Math::Vec2<int> pos, RGB rgb) {
    ::DrawText(text.c_str(), pos.X, pos.Y, 18, {rgb.R, rgb.G, rgb.B, 255});
}

void Dorian::FillBG(RGBA rgba) {
    Color SampleColor{rgba.R, rgba.G, rgba.B, rgba.A};

    ClearBackground(SampleColor);
}

// Images
Dorian::Image::Image(std::string src) {
    m_imgData = LoadTexture(src.c_str());
}

// Dorian::Image::~Image() {
    // UnloadTexture(m_imgData);
// }

void Dorian::Image::Draw(Math::Vec2<int> screenPos) {
    DrawTexture(m_imgData, screenPos.X, screenPos.Y, WHITE);
}

void Dorian::Image::Draw(Math::Vec2<int> screenPos, float Scale) {
    DrawTextureEx(m_imgData, {(float)(screenPos.X), (float)(screenPos.Y)}, 0, Scale, WHITE);
}

void Dorian::Image::DrawCenter(Math::Vec2<int> screenPos) {
    DrawTexture(m_imgData, screenPos.X - m_imgData.width/2, screenPos.Y - m_imgData.height/2, WHITE);
}

void Dorian::Image::DrawCenter(Math::Vec2<int> screenPos, float Scale) {
    DrawTextureEx(m_imgData, {(float)(screenPos.X - m_imgData.width/2*Scale), (float)(screenPos.Y - m_imgData.height/2*Scale)}, 0, Scale, WHITE);
}

// UI Components

bool Dorian::Button::Update() {
    Math::Vec2<int> anchoredPos = m_WindowP->AnchorOnScreen(m_Pos, m_Size, m_AnchorTo);
    if (GetMouseX() >= anchoredPos.X && GetMouseX() <= anchoredPos.X + m_Size.X) {
        if (GetMouseY() >= anchoredPos.Y && GetMouseY() <= anchoredPos.Y + m_Size.Y) {
            m_ButtonBGColor = {155, 155, 155};
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return true;
        }
    } else {m_ButtonBGColor = {255, 255, 255};}
    return false;
}