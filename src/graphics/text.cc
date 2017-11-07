#include "text.h"

#include "font.h"

Napi::FunctionReference Text::constructor;

void Text::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function tpl = DefineClass(env, "Text", {
        InstanceMethod("copy", &Text::Copy),
        InstanceMethod("destroy", &Text::Destroy),
        InstanceMethod("move", &Text::Move),
        InstanceMethod("rotate", &Text::Rotate),
        InstanceMethod("scaleTo", &Text::Scale),
        InstanceMethod("getTransform", &Text::GetTransform),
        InstanceMethod("getInverseTransform", &Text::GetInverseTransform),
        InstanceMethod("findCharacterPosition", &Text::FindCharacterPosition),
        InstanceMethod("getLocalBounds", &Text::GetLocalBounds),
        InstanceMethod("getGlobalBounds", &Text::GetGlobalBounds),
        InstanceAccessor("position", &Text::Position_Get, &Text::Position_Set),
        InstanceAccessor("rotation", &Text::Rotation_Get, &Text::Rotation_Set),
        InstanceAccessor("scale", &Text::Scale_Get, &Text::Scale_Set),
        InstanceAccessor("origin", &Text::Origin_Get, &Text::Origin_Set),
        InstanceAccessor("string", &Text::String_Get, &Text::String_Set),
        InstanceAccessor("font", &Text::Font_Get, &Text::Font_Set),
        InstanceAccessor("characterSize", &Text::CharacterSize_Get, &Text::CharacterSize_Set),
        InstanceAccessor("style", &Text::Style_Get, &Text::Style_Set),
        InstanceAccessor("color", &Text::Color_Get, &Text::Color_Set),
        InstanceAccessor("fillColor", &Text::FillColor_Get, &Text::FillColor_Set),
        InstanceAccessor("outlineColor", &Text::OutlineColor_Get, &Text::OutlineColor_Set),
        InstanceAccessor("outlineThickness", &Text::OutlineThickness_Get, &Text::OutlineThickness_Set)
    });

    constructor = Napi::Persistent(tpl);
    constructor.SuppressDestruct();

    exports.Set("Text", tpl);
}

Text::Text(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Text>(info) {
    if(info.Length() < 1) {
        _text = sfText_create();
    } else {
        _text = info[0].As<Napi::External<sfText>>().Data();
    }
}

Text::~Text() {
    //sfText_destroy(_text);
}

Napi::Value Text::Copy(const Napi::CallbackInfo& info) {
    sfText* sfmlText = sfText_copy(_text);
    Napi::External<sfText> externalText = Napi::External<sfText>::New(info.Env(), sfmlText);

    return constructor.New({ externalText });
}

void Text::Destroy(const Napi::CallbackInfo& info) {
    sfText_destroy(_text);
}

void Text::Move(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    Napi::Object obj = info[0].As<Napi::Object>();
    sfVector2f position{0,0};

    if(obj.Has("x")) {
        position.x = obj.Get("x").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("y")) {
        position.x = obj.Get("y").As<Napi::Number>().FloatValue();
    }

    sfText_move(_text, position);
}

void Text::Rotate(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    float angle = info[0].As<Napi::Number>().FloatValue();

    sfText_rotate(_text, angle);
}

void Text::Scale(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    Napi::Object obj = info[0].As<Napi::Object>();
    sfVector2f factor{0,0};

    if(obj.Has("x")) {
        factor.x = obj.Get("x").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("y")) {
        factor.x = obj.Get("y").As<Napi::Number>().FloatValue();
    }

    sfText_scale(_text, factor);
}

Napi::Value Text::GetTransform(const Napi::CallbackInfo& info) {
    sfTransform transform = sfText_getTransform(_text);
    Napi::Array arr = Napi::Array::New(info.Env(), 9);
    
    arr.Set((uint32_t)0, Napi::Number::New(info.Env(), transform.matrix[0]));
    arr.Set(1, Napi::Number::New(info.Env(), transform.matrix[1]));
    arr.Set(2, Napi::Number::New(info.Env(), transform.matrix[2]));
    arr.Set(3, Napi::Number::New(info.Env(), transform.matrix[3]));
    arr.Set(4, Napi::Number::New(info.Env(), transform.matrix[4]));
    arr.Set(5, Napi::Number::New(info.Env(), transform.matrix[5]));
    arr.Set(6, Napi::Number::New(info.Env(), transform.matrix[6]));
    arr.Set(7, Napi::Number::New(info.Env(), transform.matrix[7]));
    arr.Set(8, Napi::Number::New(info.Env(), transform.matrix[8]));

    return arr;
}

Napi::Value Text::GetInverseTransform(const Napi::CallbackInfo& info) {
    sfTransform transform = sfText_getInverseTransform(_text);
    Napi::Array arr = Napi::Array::New(info.Env(), 9);
    
    arr.Set((uint32_t)0, Napi::Number::New(info.Env(), transform.matrix[0]));
    arr.Set(1, Napi::Number::New(info.Env(), transform.matrix[1]));
    arr.Set(2, Napi::Number::New(info.Env(), transform.matrix[2]));
    arr.Set(3, Napi::Number::New(info.Env(), transform.matrix[3]));
    arr.Set(4, Napi::Number::New(info.Env(), transform.matrix[4]));
    arr.Set(5, Napi::Number::New(info.Env(), transform.matrix[5]));
    arr.Set(6, Napi::Number::New(info.Env(), transform.matrix[6]));
    arr.Set(7, Napi::Number::New(info.Env(), transform.matrix[7]));
    arr.Set(8, Napi::Number::New(info.Env(), transform.matrix[8]));

    return arr;
}

Napi::Value Text::FindCharacterPosition(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    int32_t charPosition = info[0].As<Napi::Number>().Int32Value();
    sfVector2f position = sfText_findCharacterPos(_text, charPosition);

    Napi::Object obj = Napi::Object::New(info.Env());
    obj.Set("x", Napi::Number::New(info.Env(), position.x));
    obj.Set("y", Napi::Number::New(info.Env(), position.y));

    return obj;
}

Napi::Value Text::GetLocalBounds(const Napi::CallbackInfo& info) {
    sfFloatRect localBounds = sfText_getLocalBounds(_text);
    Napi::Object obj = Napi::Object::New(info.Env());

    obj.Set("x", Napi::Number::New(info.Env(), localBounds.left));
    obj.Set("y", Napi::Number::New(info.Env(), localBounds.top));
    obj.Set("w", Napi::Number::New(info.Env(), localBounds.width));
    obj.Set("h", Napi::Number::New(info.Env(), localBounds.height));

    return obj;
}

Napi::Value Text::GetGlobalBounds(const Napi::CallbackInfo& info) {
    sfFloatRect globalBounds = sfText_getGlobalBounds(_text);
    Napi::Object obj = Napi::Object::New(info.Env());

    obj.Set("x", Napi::Number::New(info.Env(), globalBounds.left));
    obj.Set("y", Napi::Number::New(info.Env(), globalBounds.top));
    obj.Set("w", Napi::Number::New(info.Env(), globalBounds.width));
    obj.Set("h", Napi::Number::New(info.Env(), globalBounds.height));

    return obj;
}

// Accessors
Napi::Value Text::Position_Get(const Napi::CallbackInfo& info) {
    sfVector2f position = sfText_getPosition(_text);
    
    Napi::Object obj = Napi::Object::New(info.Env());
    obj.Set("x", Napi::Number::New(info.Env(), position.x));
    obj.Set("y", Napi::Number::New(info.Env(), position.y));

    return obj;
}

void Text::Position_Set(const Napi::CallbackInfo& info, const Napi::Value& value) { 
    Napi::Object obj = value.As<Napi::Object>();
    sfVector2f position{0,0};

    if(obj.Has("x")) {
        position.x = obj.Get("x").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("y")) {
        position.y = obj.Get("y").As<Napi::Number>().FloatValue();
    }

    sfText_setPosition(_text, position);
}

Napi::Value Text::Rotation_Get(const Napi::CallbackInfo& info) {
    float rotation = sfText_getRotation(_text);

    return Napi::Number::New(info.Env(), rotation);
}

void Text::Rotation_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    float rotation = value.As<Napi::Number>().FloatValue();

    sfText_setRotation(_text, rotation);
}

Napi::Value Text::Scale_Get(const Napi::CallbackInfo& info) {
    sfVector2f scale = sfText_getScale(_text);

    Napi::Object obj = Napi::Object::New(info.Env());
    obj.Set("x", Napi::Number::New(info.Env(), scale.x));
    obj.Set("y", Napi::Number::New(info.Env(), scale.y));

    return obj;
}

void Text::Scale_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    Napi::Object obj = value.As<Napi::Object>();
    sfVector2f scale{0,0};

    if(obj.Has("x")) {
        scale.x = obj.Get("x").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("y")) {
        scale.y = obj.Get("y").As<Napi::Number>().FloatValue();
    }

    sfText_setScale(_text, scale);
}

Napi::Value Text::Origin_Get(const Napi::CallbackInfo& info) {
    sfVector2f origin = sfText_getOrigin(_text);

    Napi::Object obj = Napi::Object::New(info.Env());
    obj.Set("x", Napi::Number::New(info.Env(), origin.x));
    obj.Set("y", Napi::Number::New(info.Env(), origin.y));

    return obj;
}

void Text::Origin_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    Napi::Object obj = value.As<Napi::Object>();
    sfVector2f origin{0,0};

    if(obj.Has("x")) {
        origin.x = obj.Get("x").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("y")) {
        origin.y = obj.Get("y").As<Napi::Number>().FloatValue();
    }

    sfText_setOrigin(_text, origin);
}

Napi::Value Text::String_Get(const Napi::CallbackInfo& info) {
    const char* string = sfText_getString(_text);

    return Napi::String::New(info.Env(), string);
}

void Text::String_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    std::string string = value.As<Napi::String>().Utf8Value();

    sfText_setString(_text, string.c_str());
}

Napi::Value Text::Font_Get(const Napi::CallbackInfo& info) {
    sfFont* sfmlFont = const_cast<sfFont*>(sfText_getFont(_text));
    Napi::External<sfFont> externalFont = Napi::External<sfFont>::New(info.Env(), sfmlFont);

    return Font::constructor.New({ externalFont });
}

void Text::Font_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    Font* font = Font::Unwrap(value.As<Napi::Object>());

    sfText_setFont(_text, font->GetFont());
}

Napi::Value Text::CharacterSize_Get(const Napi::CallbackInfo& info) {
    uint32_t characterSize = sfText_getCharacterSize(_text);

    return Napi::Number::New(info.Env(), characterSize);
}

void Text::CharacterSize_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    uint32_t characterSize = value.As<Napi::Number>().Uint32Value();

    sfText_setCharacterSize(_text, characterSize);
}

Napi::Value Text::Style_Get(const Napi::CallbackInfo& info) {
    uint32_t style = sfText_getStyle(_text);
    
    return Napi::Number::New(info.Env(), style);
}

void Text::Style_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    uint32_t style = value.As<Napi::Number>().Uint32Value();

    sfText_setStyle(_text, style);
}

Napi::Value Text::Color_Get(const Napi::CallbackInfo& info) {
    sfColor color = sfText_getColor(_text);
    Napi::Object obj = Napi::Object::New(info.Env());

    obj.Set("r", Napi::Number::New(info.Env(), color.r));
    obj.Set("g", Napi::Number::New(info.Env(), color.g));
    obj.Set("b", Napi::Number::New(info.Env(), color.b));
    obj.Set("a", Napi::Number::New(info.Env(), color.a));

    return obj;
}

void Text::Color_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    Napi::Object obj = value.As<Napi::Object>();
    sfColor color{0,0,0,0};

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

    sfText_setColor(_text, color);
}

Napi::Value Text::FillColor_Get(const Napi::CallbackInfo& info) {
    sfColor color = sfText_getFillColor(_text);
    Napi::Object obj = Napi::Object::New(info.Env());

    obj.Set("r", Napi::Number::New(info.Env(), color.r));
    obj.Set("g", Napi::Number::New(info.Env(), color.g));
    obj.Set("b", Napi::Number::New(info.Env(), color.b));
    obj.Set("a", Napi::Number::New(info.Env(), color.a));

    return obj;
}

void Text::FillColor_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    Napi::Object obj = value.As<Napi::Object>();
    sfColor color{0,0,0,0};

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

    sfText_setFillColor(_text, color);
}

Napi::Value Text::OutlineColor_Get(const Napi::CallbackInfo& info) {
    sfColor color = sfText_getOutlineColor(_text);
    Napi::Object obj = Napi::Object::New(info.Env());

    obj.Set("r", Napi::Number::New(info.Env(), color.r));
    obj.Set("g", Napi::Number::New(info.Env(), color.g));
    obj.Set("b", Napi::Number::New(info.Env(), color.b));
    obj.Set("a", Napi::Number::New(info.Env(), color.a));

    return obj;
}

void Text::OutlineColor_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    Napi::Object obj = value.As<Napi::Object>();
    sfColor color{0,0,0,0};

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

    sfText_setOutlineColor(_text, color);
}

Napi::Value Text::OutlineThickness_Get(const Napi::CallbackInfo& info) {
    float thickness = sfText_getOutlineThickness(_text);

    return Napi::Number::New(info.Env(), thickness);
}

void Text::OutlineThickness_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    float thickness = value.As<Napi::Number>().FloatValue();

    sfText_setOutlineThickness(_text, thickness);
}

const sfText* Text::GetText() {
    return _text;
}