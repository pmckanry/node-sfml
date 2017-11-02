#include "image.h"

Napi::FunctionReference Image::constructor;

void Image::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function tpl = DefineClass(env, "Image", {
        InstanceMethod("copy", &Image::Copy),
        InstanceMethod("copyImage", &Image::CopyImage),
        InstanceMethod("destroy", &Image::Destroy),
        InstanceMethod("saveToFile", &Image::SaveToFile),
        InstanceMethod("getSize", &Image::GetSize),
        InstanceMethod("createMaskFromColor", &Image::CreateMaskFromColor),
        InstanceMethod("setPixel", &Image::SetPixel),
        InstanceMethod("getPixel", &Image::GetPixel),
        InstanceMethod("getPixelPtr", &Image::GetPixelPtr),
        InstanceMethod("flipHorizontally", &Image::FlipHorizontally),
        InstanceMethod("flipVertically", &Image::FlipVertically),
        StaticMethod("createFromColor", &Image::CreateFromColor),
        StaticMethod("createFromFile", &Image::CreateFromFile),
        StaticMethod("createFromMemory", &Image::CreateFromMemory),
        StaticMethod("createFromPixels", &Image::CreateFromPixels),
        StaticMethod("createFromStream", &Image::CreateFromStream)
    });

    constructor = Napi::Persistent(tpl);
    constructor.SuppressDestruct();

    exports.Set("Image", tpl);
}

Image::Image(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Image>(info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }
    
    if(info[0].IsObject()) {
        // If Object (width, height)
        Napi::Object obj = info[0].As<Napi::Object>();
        
        uint32_t w = obj.Get("w").As<Napi::Number>().Uint32Value();
        uint32_t h = obj.Get("h").As<Napi::Number>().Uint32Value();
        
        _image = sfImage_create(w, h);
    } else {
        // Else sfTexture
        _image = info[0].As<Napi::External<sfImage>>().Data();
    }
}

Image::~Image() {
    //sfImage_destroy(_image);
}

// Methods

Napi::Value Image::Copy(const Napi::CallbackInfo& info) {
    sfImage* sfmlImage = sfImage_copy(_image);
    Napi::External<sfImage> externalImage = Napi::External<sfImage>::New(info.Env(), sfmlImage);

    return constructor.New({ externalImage });
}

Napi::Value Image::CopyImage(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    Image* image = Image::Unwrap(info[0].As<Napi::Object>());

    // TODO
    throw Napi::Error::New(info.Env(), "Not implemented");
}

void Image::Destroy(const Napi::CallbackInfo& info) {
    sfImage_destroy(_image);
}

Napi::Value Image::SaveToFile(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    std::string filename = info[0].As<Napi::String>().Utf8Value();

    sfBool success = sfImage_saveToFile(_image, filename.c_str());

    return Napi::Boolean::New(info.Env(), success != sfFalse);
}

Napi::Value Image::GetSize(const Napi::CallbackInfo& info) {
    sfVector2u size = sfImage_getSize(_image);

    Napi::Object obj = Napi::Object::New(info.Env());
    obj.Set("w", Napi::Number::New(info.Env(), size.x));
    obj.Set("h", Napi::Number::New(info.Env(), size.y));

    return obj;
}

void Image::CreateMaskFromColor(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    sfColor color {0,0,0,0};
    sfUint8 alpha = 0;
    Napi::Object colorObj = info[0].As<Napi::Object>();

    if(colorObj.Has("r")) {
        color.r = colorObj.Get("r").As<Napi::Number>().Uint32Value();
    }
    if(colorObj.Has("g")) {
        color.g = colorObj.Get("g").As<Napi::Number>().Uint32Value();
    }
    if(colorObj.Has("b")) {
        color.b = colorObj.Get("b").As<Napi::Number>().Uint32Value();
    }
    if(colorObj.Has("a")) {
        color.a = colorObj.Get("a").As<Napi::Number>().Uint32Value();
    }

    if(info.Length() > 1) {
        alpha = info[1].As<Napi::Number>().Uint32Value();
    }

    sfImage_createMaskFromColor(_image, color, alpha);
}

void Image::SetPixel(const Napi::CallbackInfo& info) {
    if(info.Length() < 2) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    Napi::Object pointObj = info[0].As<Napi::Object>();
    Napi::Object colorObj = info[1].As<Napi::Object>();

    sfColor color {0,0,0,0};
    uint32_t x = pointObj.Get("x").As<Napi::Number>().Uint32Value();
    uint32_t y = pointObj.Get("y").As<Napi::Number>().Uint32Value();

    if(colorObj.Has("r")) {
        color.r = colorObj.Get("r").As<Napi::Number>().Uint32Value();
    }
    if(colorObj.Has("g")) {
        color.g = colorObj.Get("g").As<Napi::Number>().Uint32Value();
    }
    if(colorObj.Has("b")) {
        color.b = colorObj.Get("b").As<Napi::Number>().Uint32Value();
    }
    if(colorObj.Has("a")) {
        color.a = colorObj.Get("a").As<Napi::Number>().Uint32Value();
    }

    sfImage_setPixel(_image, x, y, color);
}

Napi::Value Image::GetPixel(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    Napi::Object pointObj = info[0].As<Napi::Object>();
    Napi::Object colorObj = Napi::Object::New(info.Env());

    uint32_t x = pointObj.Get("x").As<Napi::Number>().Uint32Value();
    uint32_t y = pointObj.Get("y").As<Napi::Number>().Uint32Value();

    sfColor color = sfImage_getPixel(_image, x, y);

    colorObj.Set("r", Napi::Number::New(info.Env(), color.r));
    colorObj.Set("g", Napi::Number::New(info.Env(), color.g));
    colorObj.Set("b", Napi::Number::New(info.Env(), color.b));
    colorObj.Set("a", Napi::Number::New(info.Env(), color.a));

    return colorObj;
}

Napi::Value Image::GetPixelPtr(const Napi::CallbackInfo& info) {
    throw Napi::Error::New(info.Env(), "Not implemented");
}

void Image::FlipHorizontally(const Napi::CallbackInfo& info) {
    sfImage_flipHorizontally(_image);
}

void Image::FlipVertically(const Napi::CallbackInfo& info) {
    sfImage_flipVertically(_image);
}

// Static

Napi::Value Image::CreateFromColor(const Napi::CallbackInfo& info) {
    if(info.Length() < 2) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    Napi::Object areaObj = info[0].As<Napi::Object>();
    Napi::Object colorObj = info[1].As<Napi::Object>();

    uint32_t width = areaObj.Get("w").As<Napi::Number>().Uint32Value();
    uint32_t height = areaObj.Get("h").As<Napi::Number>().Uint32Value();

    sfColor color{0,0,0,0};
    if(colorObj.Has("r")) {
        color.r = colorObj.Get("r").As<Napi::Number>().Uint32Value();
    }
    if(colorObj.Has("g")) {
        color.g = colorObj.Get("g").As<Napi::Number>().Uint32Value();
    }
    if(colorObj.Has("b")) {
        color.b = colorObj.Get("b").As<Napi::Number>().Uint32Value();
    }
    if(colorObj.Has("a")) {
        color.a = colorObj.Get("a").As<Napi::Number>().Uint32Value();
    }

    sfImage* sfmlImage = sfImage_createFromColor(width, height, color);
    Napi::External<sfImage> externalImage = Napi::External<sfImage>::New(info.Env(), sfmlImage);

    return constructor.New({ externalImage });
}

Napi::Value Image::CreateFromPixels(const Napi::CallbackInfo& info) {
    if(info.Length() < 2) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    Napi::Object areaObj = info[0].As<Napi::Object>();
    Napi::Buffer<sfUint8> pixels = info[1].As<Napi::Buffer<sfUint8>>();

    uint32_t width = areaObj.Get("w").As<Napi::Number>().Uint32Value();
    uint32_t height = areaObj.Get("h").As<Napi::Number>().Uint32Value();

    sfImage* sfmlImage = sfImage_createFromPixels(width, height, pixels.Data());
    Napi::External<sfImage> externalImage = Napi::External<sfImage>::New(info.Env(), sfmlImage);

    return constructor.New({ externalImage });
}

Napi::Value Image::CreateFromFile(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    std::string filename = info[0].As<Napi::String>().Utf8Value();

    sfImage* sfmlImage = sfImage_createFromFile(filename.c_str());
    Napi::External<sfImage> externalImage = Napi::External<sfImage>::New(info.Env(), sfmlImage);

    return constructor.New({ externalImage });
}

Napi::Value Image::CreateFromMemory(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    Napi::Buffer<void*> data = info[0].As<Napi::Buffer<void*>>();

    sfImage* sfmlImage = sfImage_createFromMemory(data.Data(), data.Length());
    Napi::External<sfImage> externalImage = Napi::External<sfImage>::New(info.Env(), sfmlImage);

    return constructor.New({ externalImage });
}

Napi::Value Image::CreateFromStream(const Napi::CallbackInfo& info) {
    throw Napi::Error::New(info.Env(), "Not implemented");
}

const sfImage* Image::GetImage() {
    return _image;
}