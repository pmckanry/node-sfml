#ifndef SFML_NODE_GRAPHICS_SPRITE_H
#define SFML_NODE_GRAPHICS_SPRITE_H

#include <napi.h>
#include <SFML/Graphics/Sprite.h>

class Sprite : public Napi::ObjectWrap<Sprite> {
public:

    static Napi::FunctionReference constuctor;
    static void Init(Napi::Env env, Napi::Object exports);

    Sprite(const Napi::CallbackInfo& info);
    ~Sprite();

    // Methods
    Napi::Value Copy(const Napi::CallbackInfo& info);
    void Destroy(const Napi::CallbackInfo& info);
    void Move(const Napi::CallbackInfo& info);
    void Rotate(const Napi::CallbackInfo& info);
    void Scale(const Napi::CallbackInfo& info);
    Napi::Value GetTransform(const Napi::CallbackInfo& info);
    Napi::Value GetInverseTransform(const Napi::CallbackInfo& info);
    Napi::Value GetLocalBounds(const Napi::CallbackInfo& info);
    Napi::Value GetGlobalBounds(const Napi::CallbackInfo& info);

    // Accessors
    Napi::Value Position_Get(const Napi::CallbackInfo& info);
    void Position_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value Rotation_Get(const Napi::CallbackInfo& info);
    void Rotation_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value Scale_Get(const Napi::CallbackInfo& info);
    void Scale_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value Origin_Get(const Napi::CallbackInfo& info);
    void Origin_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value Texture_Get(const Napi::CallbackInfo& info);
    void Texture_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value TextureRect_Get(const Napi::CallbackInfo& info);
    void TextureRect_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value Color_Get(const Napi::CallbackInfo& info);
    void Color_Set(const Napi::CallbackInfo& info, const Napi::Value& value);

    const sfSprite* GetSprite();
private:
    sfSprite* _sprite;
};

#endif // SFML_NODE_GRAPHICS_SPRITE_H