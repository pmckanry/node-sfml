#include "view.h"

Napi::FunctionReference View::constructor;

void View::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function tpl = DefineClass(env, "View", {
        InstanceMethod("copy", &View::Copy),
        InstanceMethod("destroy", &View::Destroy),
        InstanceMethod("move", &View::Move),
        InstanceMethod("reset", &View::Reset),
        InstanceMethod("rotate", &View::Rotate),
        InstanceMethod("zoom", &View::Zoom),
        InstanceAccessor("center", &View::Center_Get, &View::Center_Set),
        InstanceAccessor("size", &View::Size_Get, &View::Size_Set),
        InstanceAccessor("rotation", &View::Rotation_Get, &View::Rotation_Set),
        InstanceAccessor("viewport", &View::Viewport_Get, &View::Viewport_Set),
    });

    constructor = Napi::Persistent(tpl);
    constructor.SuppressDestruct();

    exports.Set("View", tpl);
}

View::View(const Napi::CallbackInfo& info) : Napi::ObjectWrap<View>(info) {
    if(info.Length() == 0) {
        // Empty constructor
        _view = sfView_create();
    } else if(info[0].IsObject()) {
        // Rectangle constructor
        sfFloatRect rectangle {0,0,0,0};
        Napi::Object obj = info[0].As<Napi::Object>();

        if(obj.Has("x")) {
            rectangle.left = obj.Get("x").As<Napi::Number>().FloatValue();
        }
        if(obj.Has("y")) {
            rectangle.top = obj.Get("y").As<Napi::Number>().FloatValue();
        }
        if(obj.Has("w")) {
            rectangle.width = obj.Get("w").As<Napi::Number>().FloatValue();
        }
        if(obj.Has("h")) {
            rectangle.height = obj.Get("h").As<Napi::Number>().FloatValue();
        }

        _view = sfView_createFromRect(rectangle);
    } else {
        // Copy constuctor
        _view = info[0].As<Napi::External<sfView>>().Data();
    }
}

View::~View() {
    sfView_destroy(_view);
}

// Methods

Napi::Value View::Copy(const Napi::CallbackInfo& info) {
    sfView* sfmlView = sfView_copy(_view);

    Napi::External<sfView> externalView = Napi::External<sfView>::New(info.Env(), sfmlView);

    return constructor.New({externalView});
}

void View::Destroy(const Napi::CallbackInfo& info) {
    sfView_destroy(_view);
}

void View::Move(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    sfVector2f offset;
    Napi::Object obj = info[0].As<Napi::Object>();

    if(obj.Has("x")) {
        offset.x = obj.Get("x").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("y")) {
        offset.y = obj.Get("y").As<Napi::Number>().FloatValue();
    }

    sfView_move(_view, offset);
}

void View::Reset(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    sfFloatRect rectangle {0,0,0,0};
    Napi::Object obj = info[0].As<Napi::Object>();

    if(obj.Has("x")) {
        rectangle.left = obj.Get("x").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("y")) {
        rectangle.top = obj.Get("y").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("w")) {
        rectangle.width = obj.Get("w").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("h")) {
        rectangle.height = obj.Get("h").As<Napi::Number>().FloatValue();
    }

    sfView_reset(_view, rectangle);
}

void View::Rotate(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    float angle = info[0].As<Napi::Number>().FloatValue();

    sfView_rotate(_view, angle);
}

void View::Zoom(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    float factor = info[0].As<Napi::Number>().FloatValue();

    sfView_zoom(_view, factor);
}

// Accessors

Napi::Value View::Center_Get(const Napi::CallbackInfo& info) {
    sfVector2f center = sfView_getCenter(_view);
    Napi::Object obj = Napi::Object::New(info.Env());

    obj.Set("x", Napi::Number::New(info.Env(), center.x));
    obj.Set("y", Napi::Number::New(info.Env(), center.y));

    return obj;
}

void View::Center_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    sfVector2f center {0,0};
    Napi::Object obj = value.As<Napi::Object>();

    if(obj.Has("x")) {
        center.x = obj.Get("x").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("y")) {
        center.y = obj.Get("y").As<Napi::Number>().FloatValue();
    }

    sfView_setCenter(_view, center);
}

Napi::Value View::Size_Get(const Napi::CallbackInfo& info) {
    sfVector2f size = sfView_getSize(_view);
    Napi::Object obj = Napi::Object::New(info.Env());
    
    obj.Set("w", Napi::Number::New(info.Env(), size.x));
    obj.Set("h", Napi::Number::New(info.Env(), size.y));

    return obj;
}

void View::Size_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    sfVector2f size {0,0};
    Napi::Object obj = value.As<Napi::Object>();

    if(obj.Has("w")) {
        size.x = obj.Get("w").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("h")) {
        size.y = obj.Get("h").As<Napi::Number>().FloatValue();
    }

    sfView_setSize(_view, size);
}

Napi::Value View::Rotation_Get(const Napi::CallbackInfo& info) {
    float angle = sfView_getRotation(_view);

    return Napi::Number::New(info.Env(), angle);
}

void View::Rotation_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    float angle = value.As<Napi::Number>().FloatValue();

    sfView_setRotation(_view, angle);
}

Napi::Value View::Viewport_Get(const Napi::CallbackInfo& info) {
    sfFloatRect viewport = sfView_getViewport(_view);
    Napi::Object obj = Napi::Object::New(info.Env());

    obj.Set("x", Napi::Number::New(info.Env(), viewport.left));
    obj.Set("y", Napi::Number::New(info.Env(), viewport.top));
    obj.Set("w", Napi::Number::New(info.Env(), viewport.width));
    obj.Set("h", Napi::Number::New(info.Env(), viewport.height));

    return obj;
}

void View::Viewport_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    sfFloatRect viewport {0,0,0,0};

    Napi::Object obj = value.As<Napi::Object>();

    if(obj.Has("x")) {
        viewport.left = obj.Get("x").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("y")) {
        viewport.top = obj.Get("y").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("w")) {
        viewport.width = obj.Get("w").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("h")) {
        viewport.height = obj.Get("h").As<Napi::Number>().FloatValue();
    }

    sfView_setViewport(_view, viewport);
}

const sfView* View::GetView() {
    return _view;
}
