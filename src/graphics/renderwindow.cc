#include "renderwindow.h"

#include "view.h"
#include "sprite.h"
#include "text.h"
#include "../window/event.h"

Napi::FunctionReference RenderWindow::constructor;

void RenderWindow::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function tpl = DefineClass(env, "RenderWindow", {
        InstanceMethod("close", &RenderWindow::Close),
        InstanceMethod("destroy", &RenderWindow::Destroy),
        InstanceMethod("isOpen", &RenderWindow::IsOpen),
        InstanceMethod("getSettings", &RenderWindow::GetSettings),
        InstanceMethod("pollEvent", &RenderWindow::PollEvent),
        InstanceMethod("waitEvent", &RenderWindow::WaitEvent),
        InstanceMethod("setTitle", &RenderWindow::SetTitle),
        InstanceMethod("setIcon", &RenderWindow::SetIcon),
        InstanceMethod("setVisible", &RenderWindow::SetVisible),
        InstanceMethod("setVerticalSyncEnabled", &RenderWindow::SetVerticalSyncEnabled),
        InstanceMethod("setMouseCursorVisible", &RenderWindow::SetMouseCursorVisible),
        InstanceMethod("setMouseCursorGrabbed", &RenderWindow::SetMouseCursorGrabbed),
        InstanceMethod("setKeyRepeatEnabled", &RenderWindow::SetKeyRepeatEnabled),
        InstanceMethod("setFramerateLimit", &RenderWindow::SetFramerateLimit),
        InstanceMethod("setJoystickThreshold", &RenderWindow::SetJoystickThreshold),
        InstanceMethod("setActive", &RenderWindow::SetActive),
        InstanceMethod("requestFocus", &RenderWindow::RequestFocus),
        InstanceMethod("hasFocus", &RenderWindow::HasFocus),
        InstanceMethod("display", &RenderWindow::Display),
        InstanceMethod("clear", &RenderWindow::Clear),
        InstanceMethod("getDefaultView", &RenderWindow::GetDefaultView),
        InstanceMethod("getViewport", &RenderWindow::GetViewport),
        InstanceMethod("mapPixelToCoordinates", &RenderWindow::MapPixelToCoordinates),
        InstanceMethod("mapCoordinatesToPixel", &RenderWindow::MapCoordinatesToPixel),
        InstanceMethod("drawSprite", &RenderWindow::DrawSprite),
        InstanceMethod("drawText", &RenderWindow::DrawText),
        InstanceMethod("drawShape", &RenderWindow::DrawShape),
        InstanceMethod("drawCircleShape", &RenderWindow::DrawCircleShape),
        InstanceMethod("drawConvexShape", &RenderWindow::DrawConvexShape),
        InstanceMethod("drawRectangleShape", &RenderWindow::DrawRectangleShape),
        InstanceMethod("drawVertexArray", &RenderWindow::DrawVertexArray),
        InstanceMethod("drawPrimitives", &RenderWindow::DrawPrimitives),
        InstanceMethod("pushGLStates", &RenderWindow::PushGLStates),
        InstanceMethod("popGLStates", &RenderWindow::PopGLStates),
        InstanceMethod("resetGLStates", &RenderWindow::ResetGLStates),
        InstanceMethod("getTouchPosition", &RenderWindow::GetTouchPosition),
        InstanceAccessor("position", &RenderWindow::Position_Get, &RenderWindow::Position_Set),
        InstanceAccessor("size", &RenderWindow::Size_Get, &RenderWindow::Size_Set),
        InstanceAccessor("view", &RenderWindow::View_Get, &RenderWindow::View_Set),
        InstanceAccessor("mousePosition", &RenderWindow::MousePosition_Get, &RenderWindow::MousePosition_Set),
        StaticMethod("createFromHandle", &RenderWindow::CreateFromHandle)
    });

    constructor = Napi::Persistent(tpl);
    constructor.SuppressDestruct();

    exports.Set("RenderWindow", tpl);
}

RenderWindow::RenderWindow(const Napi::CallbackInfo& info) : Napi::ObjectWrap<RenderWindow>(info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    if(info[0].IsObject()) {
        Napi::Object options = info[0].As<Napi::Object>();

        
        std::string title = options.Get("title").As<Napi::String>().Utf8Value();
        uint32_t style = options.Get("style").As<Napi::Number>().Uint32Value();

        sfVideoMode mode {640, 480, 32};
        if(options.Has("mode")) {
            Napi::Object modeObj = options.Get("mode").As<Napi::Object>();

            if(modeObj.Has("width")) {
                mode.width = modeObj.Get("width").As<Napi::Number>().Uint32Value();
            }
            if(modeObj.Has("height")) {
                mode.height = modeObj.Get("height").As<Napi::Number>().Uint32Value();
            }
            if(modeObj.Has("bitsPerPixel")) {
                mode.bitsPerPixel = modeObj.Get("bitsPerPixel").As<Napi::Number>().Uint32Value();
            }
        }
        
        _renderWindow = sfRenderWindow_create(mode, title.c_str(), style, NULL);
    } else {
        _renderWindow = info[0].As<Napi::External<sfRenderWindow>>().Data();
    }
}

RenderWindow::~RenderWindow() {
    //sfRenderWindow_destroy(_renderWindow);
}

// Methods

void RenderWindow::Close(const Napi::CallbackInfo& info) {
    sfRenderWindow_close(_renderWindow);
}

void RenderWindow::Destroy(const Napi::CallbackInfo& info) {
    sfRenderWindow_destroy(_renderWindow);
}

Napi::Value RenderWindow::IsOpen(const Napi::CallbackInfo& info) {
    sfBool results = sfRenderWindow_isOpen(_renderWindow);

    return Napi::Boolean::New(info.Env(), results != sfFalse);
}

Napi::Value RenderWindow::GetSettings(const Napi::CallbackInfo& info) {
    Napi::Object obj = Napi::Object::New(info.Env());
    
    sfContextSettings settings = sfRenderWindow_getSettings(_renderWindow);

    obj.Set("depthBits", Napi::Number::New(info.Env(), settings.depthBits));
    obj.Set("stencilBits", Napi::Number::New(info.Env(), settings.stencilBits));
    obj.Set("antialiasingLevel", Napi::Number::New(info.Env(), settings.antialiasingLevel));
    obj.Set("majorVersion", Napi::Number::New(info.Env(), settings.majorVersion));
    obj.Set("minorVersion", Napi::Number::New(info.Env(), settings.minorVersion));
    obj.Set("attributeFlags", Napi::Number::New(info.Env(), settings.attributeFlags));
    obj.Set("sRgbCapable", Napi::Boolean::New(info.Env(), settings.sRgbCapable != sfFalse));

    return obj;
}

Napi::Value RenderWindow::PollEvent(const Napi::CallbackInfo& info) {
    sfEvent event;

    sfBool hasEvent = sfRenderWindow_pollEvent(_renderWindow, &event);

    if(hasEvent == sfFalse) {
        return info.Env().Null();
    }
    
    return GetEventObject(info.Env(), event);
}

Napi::Value RenderWindow::WaitEvent(const Napi::CallbackInfo& info) {
    sfEvent event;
    
    sfBool hasEvent = sfRenderWindow_waitEvent(_renderWindow, &event);
    
    return GetEventObject(info.Env(), event);
}

void RenderWindow::SetTitle(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    std::string title = info[0].As<Napi::String>().Utf8Value();

    sfRenderWindow_setTitle(_renderWindow, title.c_str());
}

void RenderWindow::SetIcon(const Napi::CallbackInfo& info) {
    if(info.Length() < 2) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    Napi::Object pointObj = info[0].As<Napi::Object>();
    Napi::Buffer<sfUint8> pixels = info[1].As<Napi::Buffer<sfUint8>>();

    uint32_t width = pointObj.Get("w").As<Napi::Number>().Uint32Value();
    uint32_t height = pointObj.Get("h").As<Napi::Number>().Uint32Value();

    sfRenderWindow_setIcon(_renderWindow, width, height, pixels.Data());
}

void RenderWindow::SetVisible(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    bool visible = info[0].As<Napi::Boolean>();
    sfRenderWindow_setVisible(_renderWindow, visible ? sfTrue : sfFalse);
}

void RenderWindow::SetVerticalSyncEnabled(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    bool sync = info[0].As<Napi::Boolean>();
    sfRenderWindow_setVerticalSyncEnabled(_renderWindow, sync ? sfTrue : sfFalse);
}

void RenderWindow::SetMouseCursorVisible(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    bool cursorVisible = info[0].As<Napi::Boolean>();
    sfRenderWindow_setMouseCursorVisible(_renderWindow, cursorVisible ? sfTrue : sfFalse);
}

void RenderWindow::SetMouseCursorGrabbed(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    bool cursorGrabbed = info[0].As<Napi::Boolean>();
    sfRenderWindow_setMouseCursorGrabbed(_renderWindow, cursorGrabbed ? sfTrue : sfFalse);
}

void RenderWindow::SetKeyRepeatEnabled(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    bool repeatKey = info[0].As<Napi::Boolean>();
    sfRenderWindow_setKeyRepeatEnabled(_renderWindow, repeatKey ? sfTrue : sfFalse);
}

void RenderWindow::SetFramerateLimit(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    uint32_t framerateLimit = info[0].As<Napi::Number>().Int32Value();
    sfRenderWindow_setFramerateLimit(_renderWindow, framerateLimit);
}

void RenderWindow::SetJoystickThreshold(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    float joystickThreshold = info[0].As<Napi::Number>().FloatValue();
    sfRenderWindow_setJoystickThreshold(_renderWindow, joystickThreshold);
}

void RenderWindow::SetActive(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    bool isActive = info[0].As<Napi::Boolean>();
    sfRenderWindow_setActive(_renderWindow, isActive ? sfTrue : sfFalse);
}

void RenderWindow::RequestFocus(const Napi::CallbackInfo& info) {
    sfRenderWindow_requestFocus(_renderWindow);
}

Napi::Value RenderWindow::HasFocus(const Napi::CallbackInfo& info) {
    sfBool hasFocus = sfRenderWindow_hasFocus(_renderWindow);

    return Napi::Boolean::New(info.Env(), hasFocus);
}

void RenderWindow::Display(const Napi::CallbackInfo& info) {
    sfRenderWindow_display(_renderWindow);
}

void RenderWindow::Clear(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }
    sfColor color {0,0,0,0};

    Napi::Object obj = info[0].As<Napi::Object>();

    if(obj.Has("r")) {
        color.r = obj.Get("r").As<Napi::Number>().Uint32Value();
    }
    if(obj.Has("g")) {
        color.g = obj.Get("g").As<Napi::Number>().Uint32Value();
    }
    if(obj.Has("b")) {
        color.b = obj.Get("b").As<Napi::Number>().Uint32Value();
    }
    if(obj.Has("a")) {
        color.a = obj.Get("a").As<Napi::Number>().Uint32Value();
    }

    sfRenderWindow_clear(_renderWindow, color);
}

Napi::Value RenderWindow::GetDefaultView(const Napi::CallbackInfo& info) {
    sfView* defaultView = const_cast<sfView*>(sfRenderWindow_getDefaultView(_renderWindow));
    Napi::External<sfView> externalView = Napi::External<sfView>::New(info.Env(), defaultView);

    return View::constructor.New({ externalView });
}

Napi::Value RenderWindow::GetViewport(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    View* view = View::Unwrap(info[0].As<Napi::Object>());

    sfIntRect viewport = sfRenderWindow_getViewport(_renderWindow, view->GetView());
    Napi::Object obj = Napi::Object::New(info.Env());

    obj.Set("x", Napi::Number::New(info.Env(), viewport.left));
    obj.Set("y", Napi::Number::New(info.Env(), viewport.top));
    obj.Set("w", Napi::Number::New(info.Env(), viewport.width));
    obj.Set("h", Napi::Number::New(info.Env(), viewport.height));

    return obj;
}

Napi::Value RenderWindow::MapPixelToCoordinates(const Napi::CallbackInfo& info) {
    if(info.Length() < 2) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    Napi::Object pointObj = info[0].As<Napi::Object>();
    View* view = View::Unwrap(info[1].As<Napi::Object>());

    sfVector2i point{0,0};
    point.x = pointObj.Get("x").As<Napi::Number>().Int32Value();
    point.y = pointObj.Get("y").As<Napi::Number>().Int32Value();

    sfVector2f mapPoint = sfRenderWindow_mapPixelToCoords(_renderWindow, point, view->GetView());
    Napi::Object obj = Napi::Object::New(info.Env());

    obj.Set("x", Napi::Number::New(info.Env(), mapPoint.x));
    obj.Set("y", Napi::Number::New(info.Env(), mapPoint.y));

    return obj;
}

Napi::Value RenderWindow::MapCoordinatesToPixel(const Napi::CallbackInfo& info) {
    if(info.Length() < 2) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    Napi::Object pointObj = info[0].As<Napi::Object>();
    View* view = View::Unwrap(info[1].As<Napi::Object>());
    
    sfVector2f point{0,0};
    point.x = pointObj.Get("x").As<Napi::Number>().FloatValue();
    point.y = pointObj.Get("y").As<Napi::Number>().FloatValue();
    
    sfVector2i mapPoint = sfRenderWindow_mapCoordsToPixel(_renderWindow, point, view->GetView());
    Napi::Object obj = Napi::Object::New(info.Env());

    obj.Set("x", Napi::Number::New(info.Env(), mapPoint.x));
    obj.Set("y", Napi::Number::New(info.Env(), mapPoint.y));

    return obj;
}

void RenderWindow::DrawSprite(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }
    
    Sprite* sprite = Sprite::Unwrap(info[0].As<Napi::Object>());

    sfRenderWindow_drawSprite(_renderWindow, sprite->GetSprite(), NULL);
}

void RenderWindow::DrawText(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    Text* text = Text::Unwrap(info[0].As<Napi::Object>());

    sfRenderWindow_drawText(_renderWindow, text->GetText(), NULL);
}

void RenderWindow::DrawShape(const Napi::CallbackInfo& info) {
    // TODO
}

void RenderWindow::DrawCircleShape(const Napi::CallbackInfo& info) {
    // TODO
}

void RenderWindow::DrawConvexShape(const Napi::CallbackInfo& info) {
    // TODO
}

void RenderWindow::DrawRectangleShape(const Napi::CallbackInfo& info) {
    // TODO
}

void RenderWindow::DrawVertexArray(const Napi::CallbackInfo& info) {
    // TODO
}

void RenderWindow::DrawPrimitives(const Napi::CallbackInfo& info) {
    // TODO
}

void RenderWindow::PushGLStates(const Napi::CallbackInfo& info) {
    sfRenderWindow_pushGLStates(_renderWindow);
}

void RenderWindow::PopGLStates(const Napi::CallbackInfo& info) {
    sfRenderWindow_popGLStates(_renderWindow);
}

void RenderWindow::ResetGLStates(const Napi::CallbackInfo& info) {
    sfRenderWindow_resetGLStates(_renderWindow);
}

Napi::Value RenderWindow::GetTouchPosition(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    uint32_t finger = info[0].As<Napi::Number>().Uint32Value();
    sfVector2i touchPosition = sfTouch_getPositionRenderWindow(finger, _renderWindow);

    Napi::Object obj = Napi::Object::New(info.Env());
    obj.Set("x", Napi::Number::New(info.Env(), touchPosition.x));
    obj.Set("y", Napi::Number::New(info.Env(), touchPosition.y));

    return obj;
}

// Accessors

Napi::Value RenderWindow::Position_Get(const Napi::CallbackInfo& info) {
    sfVector2i windowPosition = sfRenderWindow_getPosition(_renderWindow);

    Napi::Object obj = Napi::Object::New(info.Env());
    obj.Set("x", Napi::Number::New(info.Env(), windowPosition.x));
    obj.Set("y", Napi::Number::New(info.Env(), windowPosition.y));

    return obj;
}

void RenderWindow::Position_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    sfVector2i windowPosition {0,0};
    Napi::Object obj = info[0].As<Napi::Object>();
    if(obj.Has("w")) {
        windowPosition.x = obj.Get("w").As<Napi::Number>().Int32Value();
    }
    if(obj.Has("h")) {
        windowPosition.y = obj.Get("h").As<Napi::Number>().Int32Value();
    }

    sfRenderWindow_setPosition(_renderWindow, windowPosition);
}

Napi::Value RenderWindow::Size_Get(const Napi::CallbackInfo& info) {
    sfVector2u windowSize = sfRenderWindow_getSize(_renderWindow);

    Napi::Object obj = Napi::Object::New(info.Env());
    obj.Set("w", Napi::Number::New(info.Env(), windowSize.x));
    obj.Set("h", Napi::Number::New(info.Env(), windowSize.y));
    
    return obj;
}

void RenderWindow::Size_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    sfVector2u windowSize {0,0};
    Napi::Object obj = info[0].As<Napi::Object>();
    if(obj.Has("x")) {
        windowSize.x = obj.Get("x").As<Napi::Number>().Uint32Value();
    }
    if(obj.Has("y")) {
        windowSize.y = obj.Get("y").As<Napi::Number>().Uint32Value();
    }

    sfRenderWindow_setSize(_renderWindow, windowSize);
}

Napi::Value RenderWindow::View_Get(const Napi::CallbackInfo& info) {
    const sfView* constSfmlView = sfRenderWindow_getView(_renderWindow);
    sfView* sfmlView = const_cast<sfView*>(constSfmlView);

    Napi::External<sfView> externalView = Napi::External<sfView>::New(info.Env(), sfmlView);

    return View::constructor.New({externalView});
}

void RenderWindow::View_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    View* view = View::Unwrap(info[0].As<Napi::Object>());

    sfRenderWindow_setView(_renderWindow, view->GetView());
}

Napi::Value RenderWindow::MousePosition_Get(const Napi::CallbackInfo& info) {
    sfVector2i mousePosition = sfMouse_getPositionRenderWindow(_renderWindow);
    
    Napi::Object obj = Napi::Object::New(info.Env());
    obj.Set("x", Napi::Number::New(info.Env(), mousePosition.x));
    obj.Set("y", Napi::Number::New(info.Env(), mousePosition.y));
    
    return obj;
}

void RenderWindow::MousePosition_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    sfVector2i mousePosition {0, 0};

    Napi::Object obj = info[0].As<Napi::Object>();
    if(obj.Has("x")) {
        mousePosition.x = obj.Get("x").As<Napi::Number>().Int32Value();
    }
    if(obj.Has("y")) {
        mousePosition.y = obj.Get("y").As<Napi::Number>().Int32Value();
    }

    sfMouse_setPositionRenderWindow(mousePosition, _renderWindow);
}

const sfRenderWindow* RenderWindow::GetRenderWindow() {
    return _renderWindow;
}

Napi::Value RenderWindow::CreateFromHandle(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    Napi::External<sfWindowHandle> externalHandle = info[0].As<Napi::External<sfWindowHandle>>();

    sfWindowHandle* windowHandle = externalHandle.Data();

    sfRenderWindow* sfmlRenderWindow = sfRenderWindow_createFromHandle(*windowHandle, NULL);
    Napi::External<sfRenderWindow> externalRenderWindow = Napi::External<sfRenderWindow>::New(info.Env(), sfmlRenderWindow);

    return constructor.New({ externalRenderWindow });
}