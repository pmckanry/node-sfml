#include "text.h"

Napi::FunctionReference Text::constructor;

void Text::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function tpl = DefineClass(env, "Text", {
        InstanceMethod("copy", &Text::Copy),
        InstanceMethod("destroy", &Text::Destroy),
        InstanceMethod("move", &Text::Move),
        InstanceMethod("rotate", &Text::Rotate),
        InstanceMethod("scale", &Text::Scale),
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
    
    arr.Set(0, Napi::Number::new(info.Env(), transform.matrix[0]));
    arr.Set(1, Napi::Number::new(info.Env(), transform.matrix[1]));
    arr.Set(2, Napi::Number::new(info.Env(), transform.matrix[2]));
    arr.Set(3, Napi::Number::new(info.Env(), transform.matrix[3]));
    arr.Set(4, Napi::Number::new(info.Env(), transform.matrix[4]));
    arr.Set(5, Napi::Number::new(info.Env(), transform.matrix[5]));
    arr.Set(6, Napi::Number::new(info.Env(), transform.matrix[6]));
    arr.Set(7, Napi::Number::new(info.Env(), transform.matrix[7]));
    arr.Set(8, Napi::Number::new(info.Env(), transform.matrix[8]));

    return arr;
}

Napi::Value Text::GetInverseTransform(const Napi::CallbackInfo& info) {
    sfTransform transform = sfText_getInverseTransform(_text);
    Napi::Array arr = Napi::Array::New(info.Env(), 9);
    
    arr.Set(0, Napi::Number::new(info.Env(), transform.matrix[0]));
    arr.Set(1, Napi::Number::new(info.Env(), transform.matrix[1]));
    arr.Set(2, Napi::Number::new(info.Env(), transform.matrix[2]));
    arr.Set(3, Napi::Number::new(info.Env(), transform.matrix[3]));
    arr.Set(4, Napi::Number::new(info.Env(), transform.matrix[4]));
    arr.Set(5, Napi::Number::new(info.Env(), transform.matrix[5]));
    arr.Set(6, Napi::Number::new(info.Env(), transform.matrix[6]));
    arr.Set(7, Napi::Number::new(info.Env(), transform.matrix[7]));
    arr.Set(8, Napi::Number::new(info.Env(), transform.matrix[8]));

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

}

Napi::Value Text::GetGlobalBounds(const Napi::CallbackInfo& info) {


}