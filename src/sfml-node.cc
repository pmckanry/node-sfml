#include <napi.h>

#include "audio/music.h"
#include "audio/sound.h"
#include "graphics/font.h"
#include "graphics/image.h"
#include "graphics/renderwindow.h"
#include "graphics/sprite.h"
#include "graphics/text.h"
#include "graphics/texture.h"
#include "graphics/view.h"
#include "system/clock.h"
#include "window/window.h"

void InitSystem(Napi::Env env, Napi::Object exports) {
    Clock::Init(env, exports);
}

void InitWindow(Napi::Env env, Napi::Object exports) {
    Window::Init(env, exports);
}

void InitGraphics(Napi::Env env, Napi::Object exports) {
    Font::Init(env, exports);
    Image::Init(env, exports);
    RenderWindow::Init(env, exports);
    Sprite::Init(env, exports);
    Text::Init(env, exports);
    Texture::Init(env, exports);
    View::Init(env, exports);
}

void InitAudio(Napi::Env env, Napi::Object exports) {
    Music::Init(env, exports);
    Sound::Init(env, exports);
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    InitSystem(env, exports);
    InitWindow(env, exports);
    InitGraphics(env, exports);
    InitAudio(env, exports);

    return exports;
}

NODE_API_MODULE(SFMLnode, InitAll)