#ifndef SFML_NODE_WINDOW_WINDOW_H
#define SFML_NODE_WINDOW_WINDOW_H

#include <napi.h>

#include <SFML/Window/Window.h>

class Window : public Napi::ObjectWrap<Window> {
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);

    Window(const Napi::CallbackInfo& info);
    ~Window();

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
    
    Napi::Value Position_Get(const Napi::CallbackInfo& info);
    void Position_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value Size_Get(const Napi::CallbackInfo& info);
    void Size_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    
    sfWindowHandle GetSystemHandle();
    const sfWindow* GetWindow();

    // Static
    static Napi::Value CreateFromHandle(const Napi::CallbackInfo& info);

private:
    sfWindow* _window;
};

#endif // SFML_NODE_WINDOW_WINDOW_H