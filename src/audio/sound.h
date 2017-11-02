#ifndef SFML_NODE_AUDIO_SOUND_H
#define SFML_NODE_AUDIO_SOUND_H

#include <napi.h>
#include <SFML/Audio/Sound.h>

class Sound : public Napi::ObjectWrap<Sound> {
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);

    Sound(const Napi::CallbackInfo& info);
    ~Sound();

    // Methods
    Napi::Value Copy(const Napi::CallbackInfo& info);
    void Destroy(const Napi::CallbackInfo& info);
    void Play(const Napi::CallbackInfo& info);
    void Pause(const Napi::CallbackInfo& info);
    void Stop(const Napi::CallbackInfo& info);
    Napi::Value GetStatus(const Napi::CallbackInfo& info);

    // Accessors
    Napi::Value Buffer_Get(const Napi::CallbackInfo& info);
    void Buffer_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value Loop_Get(const Napi::CallbackInfo& info);
    void Loop_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value Pitch_Get(const Napi::CallbackInfo& info);
    void Pitch_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value Volume_Get(const Napi::CallbackInfo& info);
    void Volume_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value Position_Get(const Napi::CallbackInfo& info);
    void Position_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value RelativeToListener_Get(const Napi::CallbackInfo& info);
    void RelativeToListener_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value MinDistance_Get(const Napi::CallbackInfo& info);
    void MinDistance_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value Attenuation_Get(const Napi::CallbackInfo& info);
    void Attenuation_Set(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value PlayingOffset_Get(const Napi::CallbackInfo& info);
    void PlayingOffset_Set(const Napi::CallbackInfo& info, const Napi::Value& value);

    // Static

    const sfSound* GetSound();
private:
    sfSound* _sound;
};

#endif // SFML_NODE_AUDIO_SOUND_H