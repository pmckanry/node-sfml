#include "window.h"

#include "event.h"

Napi::FunctionReference Window::constructor;

void Window::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function tpl = DefineClass(env, "RenderWindow", {
        InstanceMethod("close", &Window::Close),
        InstanceMethod("destroy", &Window::Destroy),
        InstanceMethod("isOpen", &Window::IsOpen),
        InstanceMethod("getSettings", &Window::GetSettings),
        InstanceMethod("pollEvent", &Window::PollEvent),
        InstanceMethod("waitEvent", &Window::WaitEvent),
        InstanceMethod("setTitle", &Window::SetTitle),
        InstanceMethod("setIcon", &Window::SetIcon),
        InstanceMethod("setVisible", &Window::SetVisible),
        InstanceMethod("setVerticalSyncEnabled", &Window::SetVerticalSyncEnabled),
        InstanceMethod("setMouseCursorVisible", &Window::SetMouseCursorVisible),
        InstanceMethod("setMouseCursorGrabbed", &Window::SetMouseCursorGrabbed),
        InstanceMethod("setKeyRepeatEnabled", &Window::SetKeyRepeatEnabled),
        InstanceMethod("setFramerateLimit", &Window::SetFramerateLimit),
        InstanceMethod("setJoystickThreshold", &Window::SetJoystickThreshold),
        InstanceMethod("setActive", &Window::SetActive),
        InstanceMethod("requestFocus", &Window::RequestFocus),
        InstanceMethod("hasFocus", &Window::HasFocus),
        InstanceMethod("display", &Window::Display),
        InstanceAccessor("position", &Window::Position_Get, &Window::Position_Set),
        InstanceAccessor("size", &Window::Size_Get, &Window::Size_Set),
        StaticMethod("createFromHandle", &Window::CreateFromHandle)
    });

    constructor = Napi::Persistent(tpl);
    constructor.SuppressDestruct();

    exports.Set("Window", tpl);
}

Window::Window(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Window>(info) {
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
        
        _window = sfWindow_create(mode, title.c_str(), style, NULL);
    } else {
        _window = info[0].As<Napi::External<sfWindow>>().Data();
    }
}

Window::~Window() {
    //sfWindow_destroy(_window);
}

// Methods

void Window::Close(const Napi::CallbackInfo& info) {
    sfWindow_close(_window);
}

void Window::Destroy(const Napi::CallbackInfo& info) {
    sfWindow_destroy(_window);
}

Napi::Value Window::IsOpen(const Napi::CallbackInfo& info) {
    sfBool results = sfWindow_isOpen(_window);
    
    return Napi::Boolean::New(info.Env(), results != sfFalse);
}

Napi::Value Window::GetSettings(const Napi::CallbackInfo& info) {
    Napi::Object obj = Napi::Object::New(info.Env());
    
    sfContextSettings settings = sfWindow_getSettings(_window);

    obj.Set("depthBits", Napi::Number::New(info.Env(), settings.depthBits));
    obj.Set("stencilBits", Napi::Number::New(info.Env(), settings.stencilBits));
    obj.Set("antialiasingLevel", Napi::Number::New(info.Env(), settings.antialiasingLevel));
    obj.Set("majorVersion", Napi::Number::New(info.Env(), settings.majorVersion));
    obj.Set("minorVersion", Napi::Number::New(info.Env(), settings.minorVersion));
    obj.Set("attributeFlags", Napi::Number::New(info.Env(), settings.attributeFlags));
    obj.Set("sRgbCapable", Napi::Boolean::New(info.Env(), settings.sRgbCapable != sfFalse));

    return obj;
}

Napi::Value Window::PollEvent(const Napi::CallbackInfo& info) {
    sfEvent event;
    
    sfBool hasEvent = sfWindow_pollEvent(_window, &event);

    if(hasEvent == sfFalse) {
        return info.Env().Null();
    }

    return GetEventObject(info.Env(), event);
}

Napi::Value Window::WaitEvent(const Napi::CallbackInfo& info) {
    sfEvent event;

    sfBool hasEvent = sfWindow_waitEvent(_window, &event);

    return GetEventObject(info.Env(), event);
}

void Window::SetTitle(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    std::string title = info[0].As<Napi::String>().Utf8Value();

    sfWindow_setTitle(_window, title.c_str());
}

void Window::SetIcon(const Napi::CallbackInfo& info) {
    if(info.Length() < 2) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    Napi::Object pointObj = info[0].As<Napi::Object>();
    Napi::Buffer<sfUint8> pixels = info[1].As<Napi::Buffer<sfUint8>>();

    uint32_t width = pointObj.Get("w").As<Napi::Number>().Uint32Value();
    uint32_t height = pointObj.Get("h").As<Napi::Number>().Uint32Value();

    sfWindow_setIcon(_window, width, height, pixels.Data());
}

void Window::SetVisible(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    bool visible = info[0].As<Napi::Boolean>();
    sfWindow_setVisible(_window, visible ? sfTrue : sfFalse);
}

void Window::SetVerticalSyncEnabled(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    bool sync = info[0].As<Napi::Boolean>();
    sfWindow_setVerticalSyncEnabled(_window, sync ? sfTrue : sfFalse);
}

void Window::SetMouseCursorVisible(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    bool cursorVisible = info[0].As<Napi::Boolean>();
    sfWindow_setMouseCursorVisible(_window, cursorVisible ? sfTrue : sfFalse);
}

void Window::SetMouseCursorGrabbed(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    bool cursorGrabbed = info[0].As<Napi::Boolean>();
    sfWindow_setMouseCursorGrabbed(_window, cursorGrabbed ? sfTrue : sfFalse);
}

void Window::SetKeyRepeatEnabled(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    bool repeatKey = info[0].As<Napi::Boolean>();
    sfWindow_setKeyRepeatEnabled(_window, repeatKey ? sfTrue : sfFalse);
}

void Window::SetFramerateLimit(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    uint32_t framerateLimit = info[0].As<Napi::Number>().Int32Value();
    sfWindow_setFramerateLimit(_window, framerateLimit);
}

void Window::SetJoystickThreshold(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    float joystickThreshold = info[0].As<Napi::Number>().FloatValue();
    sfWindow_setJoystickThreshold(_window, joystickThreshold);
}

void Window::SetActive(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    bool isActive = info[0].As<Napi::Boolean>();
    sfWindow_setActive(_window, isActive ? sfTrue : sfFalse);
}

void Window::RequestFocus(const Napi::CallbackInfo& info) {
    sfWindow_requestFocus(_window);
}

Napi::Value Window::HasFocus(const Napi::CallbackInfo& info) {
    sfBool hasFocus = sfWindow_hasFocus(_window);
    
    return Napi::Boolean::New(info.Env(), hasFocus);
}

void Window::Display(const Napi::CallbackInfo& info) {
    sfWindow_display(_window);
}

// Accessors

Napi::Value Window::Position_Get(const Napi::CallbackInfo& info) {
    sfVector2i windowPosition = sfWindow_getPosition(_window);
    
    Napi::Object obj = Napi::Object::New(info.Env());
    obj.Set("x", Napi::Number::New(info.Env(), windowPosition.x));
    obj.Set("y", Napi::Number::New(info.Env(), windowPosition.y));

    return obj;
}

void Window::Position_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    sfVector2i windowPosition {0,0};
    Napi::Object obj = info[0].As<Napi::Object>();
    if(obj.Has("x")) {
        windowPosition.x = obj.Get("x").As<Napi::Number>().Int32Value();
    }
    if(obj.Has("y")) {
        windowPosition.y = obj.Get("y").As<Napi::Number>().Int32Value();
    }

    sfWindow_setPosition(_window, windowPosition);
}

Napi::Value Window::Size_Get(const Napi::CallbackInfo& info) {
    sfVector2u windowSize = sfWindow_getSize(_window);
    
    Napi::Object obj = Napi::Object::New(info.Env());
    obj.Set("w", Napi::Number::New(info.Env(), windowSize.x));
    obj.Set("h", Napi::Number::New(info.Env(), windowSize.y));
    
    return obj;
}

void Window::Size_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    sfVector2u windowSize {0,0};
    Napi::Object obj = info[0].As<Napi::Object>();
    if(obj.Has("w")) {
        windowSize.x = obj.Get("w").As<Napi::Number>().Uint32Value();
    }
    if(obj.Has("h")) {
        windowSize.y = obj.Get("h").As<Napi::Number>().Uint32Value();
    }

    sfWindow_setSize(_window, windowSize);
}


const sfWindow* Window::GetWindow() {
    return _window;
}

Napi::Value Window::CreateFromHandle(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    Napi::External<sfWindowHandle> externalHandle = info[0].As<Napi::External<sfWindowHandle>>();

    sfWindowHandle* windowHandle = externalHandle.Data();

    sfWindow* sfmlWindow = sfWindow_createFromHandle(*windowHandle, NULL);
    Napi::External<sfWindow> externalWindow = Napi::External<sfWindow>::New(info.Env(), sfmlWindow);

    return constructor.New({ externalWindow });
}