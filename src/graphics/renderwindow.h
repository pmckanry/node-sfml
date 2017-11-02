#ifndef SFML_NODE_GRAPHICS_RENDERWINDOW_H
#define SFML_NODE_GRAPHICS_RENDERWINDOW_H

#include <napi.h>

#include <SFML/Graphics/RenderWindow.h>

class RenderWindow : public Napi::ObjectWrap<RenderWindow> {
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);

    RenderWindow(const Napi::CallbackInfo& info);
    ~RenderWindow();

    // Methods

    void Close(const Napi::CallbackInfo& info);
    void Destroy(const Napi::CallbackInfo& info);
    Napi::Value IsOpen(const Napi::CallbackInfo& info);
    Napi::Value GetSettings(const Napi::CallbackInfo& info);
    Napi::Value PollEvent(const Napi::CallbackInfo& info);
    Napi::Value WaitEvent(const Napi::CallbackInfo& info);
    void SetTitle(const Napi::CallbackInfo& info);
    void SetIcon(const Napi::CallbackInfo& info);
    void SetVisible(const Napi::CallbackInfo& info);
    void SetVerticalSyncEnabled(const Napi::CallbackInfo& info);
    void SetMouseCursorVisible(const Napi::CallbackInfo& info);
    void SetMouseCursorGrabbed(const Napi::CallbackInfo& info);
    void SetKeyRepeatEnabled(const Napi::CallbackInfo& info);
    void SetFramerateLimit(const Napi::CallbackInfo& info);
    void SetJoystickThreshold(const Napi::CallbackInfo& info);
    void SetActive(const Napi::CallbackInfo& info);
    void RequestFocus(const Napi::CallbackInfo& info);
    Napi::Value HasFocus(const Napi::CallbackInfo& info);
    void Display(const Napi::CallbackInfo& info);
    void Clear(const Napi::CallbackInfo& info);
    Napi::Value GetDefaultView(const Napi::CallbackInfo& info);
    Napi::Value GetViewport(const Napi::CallbackInfo& info);

    Napi::Value MapPixelToCoordinates(const Napi::CallbackInfo& info);
    Napi::Value MapCoordinatesToPixel(const Napi::CallbackInfo& info);

    void DrawSprite(const Napi::CallbackInfo& info);
    void DrawText(const Napi::CallbackInfo& info);
    void DrawShape(const Napi::CallbackInfo& info);
    void DrawCircleShape(const Napi::CallbackInfo& info);
    void DrawConvexShape(const Napi::CallbackInfo& info);
    void DrawRectangleShape(const Napi::CallbackInfo& info);
    void DrawVertexArray(const Napi::CallbackInfo& info);
    void DrawPrimitives(const Napi::CallbackInfo& info);

    void PushGLStates(const Napi::CallbackInfo& info);
    void PopGLStates(const Napi::CallbackInfo& info);
    void ResetGLStates(const Napi::CallbackInfo& info);
    Napi::Value GetTouchPosition(const Napi::CallbackInfo& info);
    
    // Accessors
    
    Napi::Value Position_Get(const Napi::CallbackInfo& info);
    void Position_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value Size_Get(const Napi::CallbackInfo& info);
    void Size_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value View_Get(const Napi::CallbackInfo& info);
    void View_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value MousePosition_Get(const Napi::CallbackInfo& info);
    void MousePosition_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    
    sfWindowHandle GetSystemHandle(const Napi::CallbackInfo& info);
    const sfRenderWindow* GetRenderWindow();

    // Static
    static Napi::Value CreateFromHandle(const Napi::CallbackInfo& info);

private:
    sfRenderWindow* _renderWindow;
};

#endif // SFML_NODE_GRAPHICS_RENDERWINDOW_H