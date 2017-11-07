#ifndef SFML_NODE_GRAPHICS_TEXT_H
#define SFML_NODE_GRAPHICS_TEXT_H

#include <napi.h>
#include <SFML/Graphics/Text.h>

class Text : public Napi::ObjectWrap<Text> {
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);

    Text(const Napi::CallbackInfo& info);
    ~Text();

    // Methods
    Napi::Value Copy(const Napi::CallbackInfo& info);
    void Destroy(const Napi::CallbackInfo& info);
    void Move(const Napi::CallbackInfo& info);
    void Rotate(const Napi::CallbackInfo& info);
    void Scale(const Napi::CallbackInfo& info);
    Napi::Value GetTransform(const Napi::CallbackInfo& info);
    Napi::Value GetInverseTransform(const Napi::CallbackInfo& info);
    Napi::Value FindCharacterPosition(const Napi::CallbackInfo& info);
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
    
    Napi::Value String_Get(const Napi::CallbackInfo& info);
    void String_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value Font_Get(const Napi::CallbackInfo& info);
    void Font_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value CharacterSize_Get(const Napi::CallbackInfo& info);
    void CharacterSize_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value Style_Get(const Napi::CallbackInfo& info);
    void Style_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value Color_Get(const Napi::CallbackInfo& info);
    void Color_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value FillColor_Get(const Napi::CallbackInfo& info);
    void FillColor_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value OutlineColor_Get(const Napi::CallbackInfo& info);
    void OutlineColor_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value OutlineThickness_Get(const Napi::CallbackInfo& info);
    void OutlineThickness_Set(const Napi::CallbackInfo& info, const Napi::Value& value);

    const sfText* GetText();

private:
    sfText* _text;
};

#endif // SFML_NODE_GRAPHICS_TEXT_H