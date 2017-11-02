#ifndef SFML_NODE_GRAPHICS_VIEW_H
#define SFML_NODE_GRAPHICS_VIEW_H

#include <napi.h>

#include <SFML/Graphics/View.h>

class View : public Napi::ObjectWrap<View> {
public:

    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);

    View(const Napi::CallbackInfo& info);
    ~View();

    Napi::Value Copy(const Napi::CallbackInfo& info);
    void Destroy(const Napi::CallbackInfo& info);
    void Move(const Napi::CallbackInfo& info);
    void Reset(const Napi::CallbackInfo& info);
    void Rotate(const Napi::CallbackInfo& info);
    void Zoom(const Napi::CallbackInfo& info);

    Napi::Value Center_Get(const Napi::CallbackInfo& info);
    void Center_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value Size_Get(const Napi::CallbackInfo& info);
    void Size_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value Rotation_Get(const Napi::CallbackInfo& info);
    void Rotation_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value Viewport_Get(const Napi::CallbackInfo& info);
    void Viewport_Set(const Napi::CallbackInfo& info, const Napi::Value& value);

    const sfView* GetView();

private:
    sfView* _view;
};

#endif // SFML_NODE_GRAPHICS_VIEW_H