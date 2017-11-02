#include "texture.h"

#include "image.h"
#include "renderwindow.h"
#include "../window/window.h"

Napi::FunctionReference Texture::constructor;

void Texture::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function tpl = DefineClass(env, "Texture", {
        InstanceMethod("bind", &Texture::Bind),
        InstanceMethod("copy", &Texture::Copy),
        InstanceMethod("copyToImage", &Texture::CopyToImage),
        InstanceMethod("destroy", &Texture::Destroy),
        InstanceMethod("generateMipMap", &Texture::GenerateMipMap),
        InstanceMethod("getNativeHandle", &Texture::GetNativeHandle),
        InstanceMethod("getSize", &Texture::GetSize),
        InstanceMethod("updateFromPixels", &Texture::UpdateFromPixels),
        InstanceMethod("updateFromImage", &Texture::UpdateFromImage),
        InstanceMethod("updateFromWindow", &Texture::UpdateFromWindow),
        InstanceMethod("updateFromRenderWindow", &Texture::UpdateFromRenderWindow),
        InstanceAccessor("smooth", &Texture::Smooth_Get, &Texture::Smooth_Set),
        InstanceAccessor("sRGB", &Texture::SRGB_Get, &Texture::SRGB_Set),
        InstanceAccessor("repeated", &Texture::Repeated_Get, &Texture::Repeated_Set),
        StaticMethod("clearBind", &Texture::ClearBind),
        StaticMethod("createFromFile", &Texture::CreateFromFile),
        StaticMethod("createFromMemory", &Texture::CreateFromMemory),
        StaticMethod("createFromStream", &Texture::CreateFromStream),
        StaticMethod("getMaximumSize", &Texture::GetMaximumSize)
    });

    constructor = Napi::Persistent(tpl);
    constructor.SuppressDestruct();

    exports.Set("Texture", tpl);
}

Texture::Texture(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Texture>(info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }
    
    if(info[0].IsObject()) {
        // If Object (width, height)
        Napi::Object obj = info[0].As<Napi::Object>();
        
        uint32_t w = obj.Get("w").As<Napi::Number>().Uint32Value();
        uint32_t h = obj.Get("h").As<Napi::Number>().Uint32Value();
        
        _texture = sfTexture_create(w, h);
    } else {
        // Else sfTexture
        _texture = info[0].As<Napi::External<sfTexture>>().Data();
    }
}

Texture::~Texture() {
    //sfTexture_destroy(_texture);
}

void Texture::Bind(const Napi::CallbackInfo& info) {
    sfTexture_bind(_texture);
}

Napi::Value Texture::Copy(const Napi::CallbackInfo& info) {
    sfTexture* sfmlTexture = sfTexture_copy(_texture);
    Napi::External<sfTexture> externalTexture = Napi::External<sfTexture>::New(info.Env(), sfmlTexture);

    return constructor.New({ externalTexture });
}

Napi::Value Texture::CopyToImage(const Napi::CallbackInfo& info) {
    sfImage* sfmlImage = sfTexture_copyToImage(_texture);
    Napi::External<sfImage> externalImage = Napi::External<sfImage>::New(info.Env(), sfmlImage);

    return Image::constructor.New({ externalImage });
}

void Texture::Destroy(const Napi::CallbackInfo& info) {
    sfTexture_destroy(_texture);
}

Napi::Value Texture::GenerateMipMap(const Napi::CallbackInfo& info) {
    sfBool hasGenerated = sfTexture_generateMipmap(_texture);

    return Napi::Boolean::New(info.Env(), hasGenerated != sfFalse);
}

Napi::Value Texture::GetNativeHandle(const Napi::CallbackInfo& info) {
    unsigned int handle = sfTexture_getNativeHandle(_texture);

    return Napi::Number::New(info.Env(), handle);
}

Napi::Value Texture::GetSize(const Napi::CallbackInfo& info) {
    sfVector2u size = sfTexture_getSize(_texture);
    Napi::Object obj = Napi::Object::New(info.Env());

    obj.Set("w", Napi::Number::New(info.Env(),size.x));
    obj.Set("h", Napi::Number::New(info.Env(),size.y));

    return obj;
}

void Texture::UpdateFromPixels(const Napi::CallbackInfo& info) {
    if(info.Length() < 2) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    Napi::Buffer<unsigned char> pixels = info[0].As<Napi::Buffer<unsigned char>>();
    Napi::Object rectangle = info[1].As<Napi::Object>();

    uint32_t x = rectangle.Get("x").As<Napi::Number>().Uint32Value();
    uint32_t y = rectangle.Get("y").As<Napi::Number>().Uint32Value();
    uint32_t w = rectangle.Get("w").As<Napi::Number>().Uint32Value();
    uint32_t h = rectangle.Get("h").As<Napi::Number>().Uint32Value();

    sfTexture_updateFromPixels(_texture, pixels.Data(), w, h, x, y);
}

void Texture::UpdateFromImage(const Napi::CallbackInfo& info) {
    if(info.Length() < 2) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    Image* image = Image::Unwrap(info[0].As<Napi::Object>());
    Napi::Object point = info[1].As<Napi::Object>();

    unsigned int x = point.Get("x").As<Napi::Number>().Uint32Value();
    unsigned int y = point.Get("y").As<Napi::Number>().Uint32Value();

    sfTexture_updateFromImage(_texture, image->GetImage(), x, y);
}

void Texture::UpdateFromWindow(const Napi::CallbackInfo& info) {
    if(info.Length() < 2) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    Window* window = Window::Unwrap(info[0].As<Napi::Object>());
    Napi::Object point = info[1].As<Napi::Object>();

    unsigned int x = point.Get("x").As<Napi::Number>().Uint32Value();
    unsigned int y = point.Get("y").As<Napi::Number>().Uint32Value();

    sfTexture_updateFromWindow(_texture, window->GetWindow(), x, y);
}

void Texture::UpdateFromRenderWindow(const Napi::CallbackInfo& info) {
    if(info.Length() < 2) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    RenderWindow* renderWindow = RenderWindow::Unwrap(info[0].As<Napi::Object>());
    Napi::Object point = info[1].As<Napi::Object>();

    unsigned int x = point.Get("x").As<Napi::Number>().Uint32Value();
    unsigned int y = point.Get("y").As<Napi::Number>().Uint32Value();

    sfTexture_updateFromRenderWindow(_texture, renderWindow->GetRenderWindow(), x, y);
}


// Accessors
Napi::Value Texture::Smooth_Get(const Napi::CallbackInfo& info) {
    sfBool isSmooth = sfTexture_isSmooth(_texture);

    return Napi::Boolean::New(info.Env(), isSmooth != sfFalse);
}

void Texture::Smooth_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    bool isSmooth = value.As<Napi::Boolean>();

    sfTexture_setSmooth(_texture, isSmooth ? sfTrue : sfFalse);
}

Napi::Value Texture::SRGB_Get(const Napi::CallbackInfo& info) {
    sfBool isSRGB = sfTexture_isSrgb(_texture);
    
    return Napi::Boolean::New(info.Env(), isSRGB != sfFalse);
}

void Texture::SRGB_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    bool isSRGB = value.As<Napi::Boolean>();
    
    sfTexture_setSrgb(_texture, isSRGB ? sfTrue : sfFalse);
}

Napi::Value Texture::Repeated_Get(const Napi::CallbackInfo& info) {
    sfBool isRepeated = sfTexture_isRepeated(_texture);
    
    return Napi::Boolean::New(info.Env(), isRepeated != sfFalse);
}

void Texture::Repeated_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    bool isRepeated = value.As<Napi::Boolean>();
    
    sfTexture_setRepeated(_texture, isRepeated ? sfTrue : sfFalse);
}

void Texture::ClearBind(const Napi::CallbackInfo& info) {
    sfTexture_bind(NULL);
}

Napi::Value Texture::CreateFromFile(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }
    
    sfIntRect area;
    bool hasArea = false;
    std::string filename = info[0].As<Napi::String>().Utf8Value();

    if(info.Length() > 1) {
        hasArea = true;
        Napi::Object areaObj = info[1].As<Napi::Object>();

        area.left = areaObj.Get("x").As<Napi::Number>().Int32Value();
        area.top = areaObj.Get("y").As<Napi::Number>().Int32Value();
        area.width = areaObj.Get("w").As<Napi::Number>().Int32Value();
        area.height = areaObj.Get("h").As<Napi::Number>().Int32Value();
    }

    sfTexture* sfmlTexture = sfTexture_createFromFile(filename.c_str(), hasArea ? &area : NULL);
    Napi::External<sfTexture> externalTexture = Napi::External<sfTexture>::New(info.Env(), sfmlTexture);

    return constructor.New({ externalTexture });
}

Napi::Value Texture::CreateFromMemory(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    sfIntRect area;
    bool hasArea = false;
    
    Napi::Buffer<void*> data = info[0].As<Napi::Buffer<void*>>();

    if(info.Length() > 1) {
        hasArea = true;
        Napi::Object areaObj = info[1].As<Napi::Object>();

        area.left = areaObj.Get("x").As<Napi::Number>().Int32Value();
        area.top = areaObj.Get("y").As<Napi::Number>().Int32Value();
        area.width = areaObj.Get("w").As<Napi::Number>().Int32Value();
        area.height = areaObj.Get("h").As<Napi::Number>().Int32Value();
    }

    sfTexture* sfmlTexture = sfTexture_createFromMemory(data.Data(), data.Length(), hasArea ? &area : NULL);
    Napi::External<sfTexture> externalTexture = Napi::External<sfTexture>::New(info.Env(), sfmlTexture);

    return constructor.New({ externalTexture });
}

Napi::Value Texture::CreateFromStream(const Napi::CallbackInfo& info) {
    throw Napi::Error::New(info.Env(), "Not implemented");
}

Napi::Value Texture::GetMaximumSize(const Napi::CallbackInfo& info) {
    unsigned int maxSize = sfTexture_getMaximumSize();

    return Napi::Number::New(info.Env(), maxSize);
}

const sfTexture* Texture::GetTexture() {
    return _texture;
}