#ifndef SFML_NODE_GRAPHICS_IMAGE_H
#define SFML_NODE_GRAPHICS_IMAGE_H

#include <napi.h>
#include <SFML/Graphics/Image.h>

class Image : public Napi::ObjectWrap<Image> {
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);

    Image(const Napi::CallbackInfo& info);
    ~Image();

    Napi::Value Copy(const Napi::CallbackInfo& info);
    Napi::Value CopyImage(const Napi::CallbackInfo& info);
    void Destroy(const Napi::CallbackInfo& info);
    Napi::Value SaveToFile(const Napi::CallbackInfo& info);
    Napi::Value GetSize(const Napi::CallbackInfo& info);
    void CreateMaskFromColor(const Napi::CallbackInfo& info);
    void SetPixel(const Napi::CallbackInfo& info);
    Napi::Value GetPixel(const Napi::CallbackInfo& info);
    Napi::Value GetPixelPtr(const Napi::CallbackInfo& info);
    void FlipHorizontally(const Napi::CallbackInfo& info);
    void FlipVertically(const Napi::CallbackInfo& info);

    static Napi::Value CreateFromColor(const Napi::CallbackInfo& info);
    static Napi::Value CreateFromPixels(const Napi::CallbackInfo& info);
    static Napi::Value CreateFromFile(const Napi::CallbackInfo& info);
    static Napi::Value CreateFromMemory(const Napi::CallbackInfo& info);
    static Napi::Value CreateFromStream(const Napi::CallbackInfo& info);

    const sfImage* GetImage();
private:
    sfImage* _image;
};

#endif // SFML_NODE_GRAPHICS_IMAGE_H