#ifndef SFML_NODE_GRAPHICS_TEXTURE_H
#define SFML_NODE_GRAPHICS_TEXTURE_H

#include <napi.h>

#include <SFML/Graphics/Texture.h>

class Texture : public Napi::ObjectWrap<Texture> {
public:

    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);

    Texture(const Napi::CallbackInfo& info);
    ~Texture();

    // Methods
    void Destroy(const Napi::CallbackInfo& info);
    void Bind(const Napi::CallbackInfo& info);
    Napi::Value Copy(const Napi::CallbackInfo& info);
    Napi::Value CopyToImage(const Napi::CallbackInfo& info);
    Napi::Value GenerateMipMap(const Napi::CallbackInfo& info);
    Napi::Value GetNativeHandle(const Napi::CallbackInfo& info);
    Napi::Value GetSize(const Napi::CallbackInfo& info);

    void UpdateFromPixels(const Napi::CallbackInfo& info);
    void UpdateFromImage(const Napi::CallbackInfo& info);
    void UpdateFromWindow(const Napi::CallbackInfo& info);
    void UpdateFromRenderWindow(const Napi::CallbackInfo& info);

    // Accessors
    Napi::Value Smooth_Get(const Napi::CallbackInfo& info);
    void Smooth_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value SRGB_Get(const Napi::CallbackInfo& info);
    void SRGB_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value Repeated_Get(const Napi::CallbackInfo& info);
    void Repeated_Set(const Napi::CallbackInfo& info, const Napi::Value& value);

    // Static
    static void ClearBind(const Napi::CallbackInfo& info);
    static Napi::Value CreateFromFile(const Napi::CallbackInfo& info);
    static Napi::Value CreateFromMemory(const Napi::CallbackInfo& info);
    static Napi::Value CreateFromStream(const Napi::CallbackInfo& info);
    static Napi::Value GetMaximumSize(const Napi::CallbackInfo& info);

    const sfTexture* GetTexture();
private:
    sfTexture* _texture;
};

#endif // SFML_NODE_GRAPHICS_TEXTURE_H