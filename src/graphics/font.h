#ifndef SFML_NODE_GRAPHICS_FONT_H
#define SFML_NODE_GRAPHICS_FONT_H

#include <napi.h>
#include <SFML/Graphics/Font.h>

class Font : public Napi::ObjectWrap<Font> {
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);

    Font(const Napi::CallbackInfo& info);
    ~Font();

    // Methods
    Napi::Value Copy(const Napi::CallbackInfo& info);
    void Destroy(const Napi::CallbackInfo& info);
    Napi::Value GetGlyph(const Napi::CallbackInfo& info);
    Napi::Value GetKerning(const Napi::CallbackInfo& info);
    Napi::Value GetLineSpacing(const Napi::CallbackInfo& info);
    Napi::Value GetUnderlinePosition(const Napi::CallbackInfo& info);
    Napi::Value GetUnderlineThickness(const Napi::CallbackInfo& info);
    Napi::Value GetTexture(const Napi::CallbackInfo& info);
    Napi::Value GetInfo(const Napi::CallbackInfo& info);

    // Static
    static Napi::Value CreateFromMemory(const Napi::CallbackInfo& info);
    static Napi::Value CreateFromStream(const Napi::CallbackInfo& info);

    const sfFont* GetFont();
private:
    sfFont* _font;
};

#endif // SFML_NODE_GRAPHICS_FONT_H