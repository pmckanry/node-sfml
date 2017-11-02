#ifndef SFML_NODE_AUDIO_SOUNDBUFFER_H
#define SFML_NODE_AUDIO_SOUNDBUFFER_H

#include <napi.h>
#include <SFML/Audio/SoundBuffer.h>

class SoundBuffer : public Napi::ObjectWrap<SoundBuffer> {
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);

    SoundBuffer(const Napi::CallbackInfo& info);
    ~SoundBuffer();

    // Methods
    Napi::Value Copy(const Napi::CallbackInfo& info);
    void Destroy(const Napi::CallbackInfo& info);
    Napi::Value SaveToFile(const Napi::CallbackInfo& info);
    Napi::Value GetSamples(const Napi::CallbackInfo& info);
    Napi::Value GetSampleCount(const Napi::CallbackInfo& info);
    Napi::Value GetSampleRate(const Napi::CallbackInfo& info);
    Napi::Value GetChannelCount(const Napi::CallbackInfo& info);
    Napi::Value GetDuration(const Napi::CallbackInfo& info);

    // Accessors
    // Static
    static Napi::Value CreateFromMemory(const Napi::CallbackInfo& info);
    static Napi::Value CreateFromStream(const Napi::CallbackInfo& info);
    static Napi::Value CreateFromSamples(const Napi::CallbackInfo& info);

    const sfSoundBuffer* GetSoundBuffer();
private:
    sfSoundBuffer* _soundBuffer;
};

#endif // SFML_NODE_AUDIO_SOUNDBUFFER_H