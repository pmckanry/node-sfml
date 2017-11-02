#include "sprite.h"

#include "texture.h"

Napi::FunctionReference Sprite::constuctor;

void Sprite::Init(Napi::Env env, Napi::Object exports) {

}

Sprite::Sprite(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Sprite>(info) {
    if(info.Length() < 1) {
        _sprite = sfSprite_create();
    } else {
        _sprite = info[0].As<Napi::External<sfSprite>>().Data();
    }
}

Sprite::~Sprite() {
    //sfSprite_destroy(_sprite);
}

Napi::Value Sprite::Copy(const Napi::CallbackInfo& info) {
    sfSprite* sfmlSprite = sfSprite_copy(_sprite);
    Napi::External<sfSprite> externalSprite = Napi::External<sfSprite>::New(info.Env(), sfmlSprite);

    return constuctor.New({ externalSprite });
}

void Sprite::Destroy(const Napi::CallbackInfo& info) {
    sfSprite_destroy(_sprite);
}

void Sprite::Move(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    Napi::Object offsetObj = info[0].As<Napi::Object>();
    sfVector2f offset {0,0};

    if(offsetObj.Has("x")) {
        offset.x = offsetObj.Get("x").As<Napi::Number>().FloatValue();
    }
    if(offsetObj.Has("y")) {
        offset.y = offsetObj.Get("y").As<Napi::Number>().FloatValue();
    }

    sfSprite_move(_sprite, offset);
}

void Sprite::Rotate(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    float angle = info[0].As<Napi::Number>().FloatValue();

    sfSprite_rotate(_sprite, angle);
}

void Sprite::Scale(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }

    Napi::Object factorsObj = info[0].As<Napi::Object>();
    sfVector2f factors {1,1};

    if(factorsObj.Has("x")) {
        factors.x = factorsObj.Get("x").As<Napi::Number>().FloatValue();
    }
    if(factorsObj.Has("y")) {
        factors.y = factorsObj.Get("y").As<Napi::Number>().FloatValue();
    }

    sfSprite_scale(_sprite, factors);
}

Napi::Value Sprite::GetTransform(const Napi::CallbackInfo& info) {
    sfTransform transform = sfSprite_getTransform(_sprite);
    Napi::Array arrayObj = Napi::Array::New(info.Env(), 9);

    arrayObj.Set((uint32_t)0, Napi::Number::New(info.Env(), transform.matrix[0]));
    arrayObj.Set((uint32_t)1, Napi::Number::New(info.Env(), transform.matrix[1]));
    arrayObj.Set((uint32_t)2, Napi::Number::New(info.Env(), transform.matrix[2]));
    arrayObj.Set((uint32_t)3, Napi::Number::New(info.Env(), transform.matrix[3]));
    arrayObj.Set((uint32_t)4, Napi::Number::New(info.Env(), transform.matrix[4]));
    arrayObj.Set((uint32_t)5, Napi::Number::New(info.Env(), transform.matrix[5]));
    arrayObj.Set((uint32_t)6, Napi::Number::New(info.Env(), transform.matrix[6]));
    arrayObj.Set((uint32_t)7, Napi::Number::New(info.Env(), transform.matrix[7]));
    arrayObj.Set((uint32_t)8, Napi::Number::New(info.Env(), transform.matrix[8]));
    arrayObj.Set((uint32_t)9, Napi::Number::New(info.Env(), transform.matrix[9]));

    return arrayObj;
}

Napi::Value Sprite::GetInverseTransform(const Napi::CallbackInfo& info) {
    sfTransform transform = sfSprite_getInverseTransform(_sprite);
    Napi::Array arrayObj = Napi::Array::New(info.Env(), 9);
    
    arrayObj.Set((uint32_t)0, Napi::Number::New(info.Env(), transform.matrix[0]));
    arrayObj.Set((uint32_t)1, Napi::Number::New(info.Env(), transform.matrix[1]));
    arrayObj.Set((uint32_t)2, Napi::Number::New(info.Env(), transform.matrix[2]));
    arrayObj.Set((uint32_t)3, Napi::Number::New(info.Env(), transform.matrix[3]));
    arrayObj.Set((uint32_t)4, Napi::Number::New(info.Env(), transform.matrix[4]));
    arrayObj.Set((uint32_t)5, Napi::Number::New(info.Env(), transform.matrix[5]));
    arrayObj.Set((uint32_t)6, Napi::Number::New(info.Env(), transform.matrix[6]));
    arrayObj.Set((uint32_t)7, Napi::Number::New(info.Env(), transform.matrix[7]));
    arrayObj.Set((uint32_t)8, Napi::Number::New(info.Env(), transform.matrix[8]));
    arrayObj.Set((uint32_t)9, Napi::Number::New(info.Env(), transform.matrix[9]));

    return arrayObj;
}

Napi::Value Sprite::GetLocalBounds(const Napi::CallbackInfo& info) {
    sfFloatRect localBounds = sfSprite_getLocalBounds(_sprite);
    Napi::Object obj = Napi::Object::New(info.Env());

    obj.Set("x", Napi::Number::New(info.Env(), localBounds.left));
    obj.Set("y", Napi::Number::New(info.Env(), localBounds.top));
    obj.Set("w", Napi::Number::New(info.Env(), localBounds.width));
    obj.Set("h", Napi::Number::New(info.Env(), localBounds.height));

    return obj;
}

Napi::Value Sprite::GetGlobalBounds(const Napi::CallbackInfo& info) {
    sfFloatRect globalBounds = sfSprite_getGlobalBounds(_sprite);
    Napi::Object obj = Napi::Object::New(info.Env());

    obj.Set("x", Napi::Number::New(info.Env(), globalBounds.left));
    obj.Set("y", Napi::Number::New(info.Env(), globalBounds.top));
    obj.Set("w", Napi::Number::New(info.Env(), globalBounds.width));
    obj.Set("h", Napi::Number::New(info.Env(), globalBounds.height));

    return obj;
}

// Accessors

Napi::Value Sprite::Position_Get(const Napi::CallbackInfo& info) {
    sfVector2f position = sfSprite_getPosition(_sprite);
    Napi::Object obj = Napi::Object::New(info.Env());

    obj.Set("x", Napi::Number::New(info.Env(), position.x));
    obj.Set("y", Napi::Number::New(info.Env(), position.y));

    return obj;
}

void Sprite::Position_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    Napi::Object obj = value.As<Napi::Object>();
    sfVector2f position{0,0};

    if(obj.Has("x")) {
        position.x = obj.Get("x").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("y")) {
        position.y = obj.Get("y").As<Napi::Number>().FloatValue();
    }

    sfSprite_setPosition(_sprite, position);
}

Napi::Value Sprite::Rotation_Get(const Napi::CallbackInfo& info) {
    float sfmlRotation = sfSprite_getRotation(_sprite);
    Napi::Number rotation = Napi::Number::New(info.Env(), sfmlRotation);

    return rotation;
}

void Sprite::Rotation_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    Napi::Number rotation = value.As<Napi::Number>();
    float sfmlRotation = rotation.FloatValue();

    sfSprite_setRotation(_sprite, sfmlRotation);
}

Napi::Value Sprite::Scale_Get(const Napi::CallbackInfo& info) {
    sfVector2f sfmlScale = sfSprite_getScale(_sprite);
    Napi::Object obj = Napi::Object::New(info.Env());
    
    obj.Set("x", Napi::Number::New(info.Env(), sfmlScale.x));
    obj.Set("y", Napi::Number::New(info.Env(), sfmlScale.y));

    return obj;
}

void Sprite::Scale_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    Napi::Object obj = value.As<Napi::Object>();
    sfVector2f scale{0,0};

    if(obj.Has("x")) {
        scale.x = obj.Get("x").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("y")) {
        scale.y = obj.Get("y").As<Napi::Number>().FloatValue();
    }

    sfSprite_setScale(_sprite, scale);
}

Napi::Value Sprite::Origin_Get(const Napi::CallbackInfo& info) {
    sfVector2f origin = sfSprite_getOrigin(_sprite);
    Napi::Object obj = Napi::Object::New(info.Env());
    
    obj.Set("x", Napi::Number::New(info.Env(), origin.x));
    obj.Set("y", Napi::Number::New(info.Env(), origin.y));

    return obj;
}

void Sprite::Origin_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    Napi::Object obj = value.As<Napi::Object>();
    sfVector2f origin{0,0};

    if(obj.Has("x")) {
        origin.x = obj.Get("x").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("y")) {
        origin.y = obj.Get("y").As<Napi::Number>().FloatValue();
    }

    sfSprite_setOrigin(_sprite, origin);
}

Napi::Value Sprite::Texture_Get(const Napi::CallbackInfo& info) {
    sfTexture* sfmlTexture = const_cast<sfTexture*>(sfSprite_getTexture(_sprite));
    Napi::External<sfTexture> externalTexture = Napi::External<sfTexture>::New(info.Env(), sfmlTexture);

    return Texture::constructor.New({ externalTexture });
}

void Sprite::Texture_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    Napi::Object obj = value.As<Napi::Object>();
    Texture* texture = Texture::Unwrap(obj);

    sfSprite_setTexture(_sprite, texture->GetTexture(), sfFalse);
}

Napi::Value Sprite::TextureRect_Get(const Napi::CallbackInfo& info) {
    sfIntRect textureRect = sfSprite_getTextureRect(_sprite);
    Napi::Object obj = Napi::Object::New(info.Env());

    obj.Set("x", Napi::Number::New(info.Env(), textureRect.left));
    obj.Set("y", Napi::Number::New(info.Env(), textureRect.top));
    obj.Set("w", Napi::Number::New(info.Env(), textureRect.width));
    obj.Set("h", Napi::Number::New(info.Env(), textureRect.height));

    return obj;
}

void Sprite::TextureRect_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    Napi::Object obj = value.As<Napi::Object>();
    sfIntRect textureRect{0,0,0,0};

    obj.Set("x", Napi::Number::New(info.Env(), textureRect.left));
    obj.Set("y", Napi::Number::New(info.Env(), textureRect.top));
    obj.Set("w", Napi::Number::New(info.Env(), textureRect.width));
    obj.Set("h", Napi::Number::New(info.Env(), textureRect.height));

    sfSprite_setTextureRect(_sprite, textureRect);
}

Napi::Value Sprite::Color_Get(const Napi::CallbackInfo& info) {
    sfColor color = sfSprite_getColor(_sprite);
    Napi::Object obj = Napi::Object::New(info.Env());

    obj.Set("r", Napi::Number::New(info.Env(), color.r));
    obj.Set("g", Napi::Number::New(info.Env(), color.g));
    obj.Set("b", Napi::Number::New(info.Env(), color.b));
    obj.Set("a", Napi::Number::New(info.Env(), color.a));

    return obj;
}

void Sprite::Color_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
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

    sfSprite_setColor(_sprite, color);
}

const sfSprite* Sprite::GetSprite() {
    return _sprite;
}