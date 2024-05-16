#include "editor.h"
#include "body.h"
#include "render.h"
#define RAYGUI_IMPLEMENTATION
#include "../../raygui/src/raygui.h"

bool ncEditorActive = true;
bool ncEditorIntersect = false;
ncEditorData_t ncEditorData;

Rectangle editorRect;

Vector2 anchor01 = { 950, 50 };
Texture2D cursorTexture;

bool EditorBoxActive = true;
float MassMinValue = 0.0f;
float MassMaxValue = 0.0f;
float GravitationValue = 0.0f;

void InitEditor()
{
    GuiLoadStyle("raygui/styles/dark/style_dark.rgs");

    Image image = LoadImage("resources/reticle.png");
    cursorTexture = LoadTextureFromImage(image);
    UnloadImage(image);

    ncEditorData.EditorBoxActive = true;

    // body 
    ncEditorData.BodyTypeEditMode = false;
    ncEditorData.BodyTypeActive = 0;
    ncEditorData.MassValue = 2.0f;
    ncEditorData.DampingValue = 0.0f;
    ncEditorData.GravityScaleValue = 1.0f;
    ncEditorData.kValue = 10.0f;

    //world
    ncEditorData.GravityValue = 0.0;
    ncEditorData.GravitationValue = 0.0f;

    ncEditorData.anchor01 = anchor01;
    editorRect = (Rectangle){ ncEditorData.anchor01.x + 0, ncEditorData.anchor01.y + 0, 304, 616 };

    HideCursor();
}

void UpdateEditor(Vector2 position)
{
    // toggle show / hide editor box with key press
    if (IsKeyPressed(KEY_TAB)) ncEditorData.EditorBoxActive = !ncEditorData.EditorBoxActive;

    // check if cursor position is intersecting the editor box  
    ncEditorIntersect = ncEditorData.EditorBoxActive && CheckCollisionPointRec(position, editorRect);
}

void DrawEditor(Vector2 position)
{
    if (ncEditorData.BodyTypeEditMode) GuiLock();

    if (ncEditorData.EditorBoxActive)
    {
        ncEditorData.EditorBoxActive = !GuiWindowBox((Rectangle) { ncEditorData.anchor01.x + 0, ncEditorData.anchor01.y + 0, 310, 584 }, "Editor");
        GuiGroupBox((Rectangle) { ncEditorData.anchor01.x + 24, ncEditorData.anchor01.y + 40, 262, 152 }, "Body");
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 96, 120, 16 }, "Mass", TextFormat("% 0.2f", ncEditorData.MassValue), & ncEditorData.MassValue, 0, 5);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 120, 120, 16 }, "Damping", TextFormat("% 0.2f", ncEditorData.DampingValue), & ncEditorData.DampingValue, 0, 10);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 144, 120, 16 }, "Gravity Scale", TextFormat("%0.2f", ncEditorData.GravityScaleValue), & ncEditorData.GravityScaleValue, 0, 10);
        GuiSliderBar((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 168, 120, 16 }, "Stiffness (k)", TextFormat("%0.2f", ncEditorData.kValue), & ncEditorData.kValue, 0, 10);
        GuiGroupBox((Rectangle) { ncEditorData.anchor01.x + 24, ncEditorData.anchor01.y + 216, 262, 104 }, "World");
        GuiSlider((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 232, 120, 16 }, "Gravity", TextFormat("%0.2f", ncEditorData.GravityValue), & ncEditorData.GravityValue, -10, 10);
        GuiSlider((Rectangle) { ncEditorData.anchor01.x + 120, ncEditorData.anchor01.y + 256, 120, 16 }, "Gravitation", TextFormat("%0.2f", ncEditorData.GravitationValue), & ncEditorData.GravitationValue, -10, 10);
        if (GuiDropdownBox((Rectangle) { ncEditorData.anchor01.x + 40, ncEditorData.anchor01.y + 56, 200, 24 }, "DYNAMIC;KINEMATIC;STATIC", & ncEditorData.BodyTypeActive, ncEditorData.BodyTypeEditMode)) ncEditorData.BodyTypeEditMode = !ncEditorData.BodyTypeEditMode;
    }

    GuiUnlock();

    DrawTexture(cursorTexture, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, WHITE);
}

ncBody* GetBodyIntersect(ncBody* bodies, Vector2 position)
{
    for (ncBody* body = bodies; body; body = body->next)
    {
        Vector2 screen = ConvertWorldToScreen(body->position);
        if (CheckCollisionPointCircle(position, screen, ConvertWorldToPixel(body->mass * 0.5f)))
        {
            return body;
        }
    }

    return NULL;
}

void DrawLineBodyToPosition(ncBody* body, Vector2 position)
{
    Vector2 screen = ConvertWorldToScreen(body->position);
    DrawLine((int)screen.x, (int)screen.y, (int)position.x - cursorTexture.width / 2, (int)position.y - cursorTexture.height / 2, YELLOW);
}
