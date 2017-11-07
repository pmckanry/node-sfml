#include "font.h"

#include "texture.h"

Napi::FunctionReference Font::constructor;

void Font::Init(Napi::Env env, Napi::Object exports) {

}

Font::Font(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Font>(info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    if(info[0].IsString()) {
        std::string filename = info[0].As<Napi::String>().Utf8Value();
        _font = sfFont_createFromFile(filename.c_str());
    } else {
        _font = info[0].As<Napi::External<sfFont>>().Data();
    }
}

Font::~Font() {
    //sfFont_destroy(_font);
}

// Methods
Napi::Value Font::Copy(const Napi::CallbackInfo& info) {
    sfFont* sfmlFont = sfFont_copy(_font);
    Napi::External<sfFont> externalFont = Napi::External<sfFont>::New(info.Env(), sfmlFont);

    return constructor.New({ externalFont });
}

void Font::Destroy(const Napi::CallbackInfo& info) {
    sfFont_destroy(_font);
}

Napi::Value Font::GetGlyph(const Napi::CallbackInfo& info) {
    throw Napi::Error::New(info.Env(), "Not implemented");
}

Napi::Value Font::GetKerning(const Napi::CallbackInfo& info) {
    if(info.Length() < 3) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    uint32_t characterSize = info[0].As<Napi::Number>().Uint32Value();
    uint32_t character1 = info[1].As<Napi::Number>().Uint32Value();
    uint32_t character2 = info[2].As<Napi::Number>().Uint32Value();

    float kerning = sfFont_getKerning(_font, character1, character2, characterSize);

    return Napi::Number::New(info.Env(), kerning);
}

Napi::Value Font::GetLineSpacing(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    uint32_t characterSize = info[0].As<Napi::Number>().Uint32Value();
    float lineSpacing = sfFont_getLineSpacing(_font, characterSize);

    return Napi::Number::New(info.Env(), lineSpacing);
}

Napi::Value Font::GetUnderlinePosition(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    uint32_t characterSize = info[0].As<Napi::Number>().Uint32Value();
    float underlinePosition = sfFont_getUnderlinePosition(_font, characterSize);

    return Napi::Number::New(info.Env(), underlinePosition);
}

Napi::Value Font::GetUnderlineThickness(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    uint32_t characterSize = info[0].As<Napi::Number>().Uint32Value();
    float thickness = sfFont_getUnderlineThickness(_font, characterSize);

    return Napi::Number::New(info.Env(), thickness);
}

Napi::Value Font::GetTexture(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    uint32_t characterSize = info[0].As<Napi::Number>().Uint32Value();

    sfTexture* sfmlTexture = const_cast<sfTexture*>(sfFont_getTexture(_font, characterSize));
    Napi::External<sfTexture> externalTexture = Napi::External<sfTexture>::New(info.Env(), sfmlTexture);

    return Texture::constructor.New({ externalTexture });
}

Napi::Value Font::GetInfo(const Napi::CallbackInfo& info) {
    sfFontInfo fontInfo = sfFont_getInfo(_font);

    return Napi::String::New(info.Env(), fontInfo.family);
}

// Static
Napi::Value Font::CreateFromMemory(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }
    
    Napi::Buffer<void*> data = info[0].As<Napi::Buffer<void*>>();

    sfFont* sfmlFont = sfFont_createFromMemory(data.Data(), data.Length());
    Napi::External<sfFont> externalFont = Napi::External<sfFont>::New(info.Env(), sfmlFont);

    return constructor.New({ externalFont });
}

Napi::Value Font::CreateFromStream(const Napi::CallbackInfo& info) {
    throw Napi::Error::New(info.Env(), "Not implemented");
}

const sfFont* Font::GetFont() {
    return _font;
}