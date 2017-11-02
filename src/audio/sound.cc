#include "sound.h"

#include "soundbuffer.h"

Napi::FunctionReference Sound::constructor;

void Sound::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function tpl = DefineClass(env, "Sound", {
        InstanceMethod("copy", &Sound::Copy),
        InstanceMethod("destroy", &Sound::Destroy),
        InstanceMethod("play", &Sound::Play),
        InstanceMethod("pause", &Sound::Pause),
        InstanceMethod("stop", &Sound::Stop),
        InstanceMethod("getStatus", &Sound::GetStatus),
        InstanceAccessor("buffer", &Sound::Buffer_Get, &Sound::Buffer_Set),
        InstanceAccessor("loop", &Sound::Loop_Get, &Sound::Loop_Set),
        InstanceAccessor("pitch", &Sound::Pitch_Get, &Sound::Pitch_Set),
        InstanceAccessor("volumne", &Sound::Volume_Get, &Sound::Volume_Set),
        InstanceAccessor("position", &Sound::Position_Get, &Sound::Position_Set),
        InstanceAccessor("relativeToListener", &Sound::RelativeToListener_Get, &Sound::RelativeToListener_Set),
        InstanceAccessor("minDistance", &Sound::MinDistance_Get, &Sound::MinDistance_Set),
        InstanceAccessor("attenuation", &Sound::Attenuation_Get, &Sound::Attenuation_Set),
        InstanceAccessor("playingOffset", &Sound::PlayingOffset_Get, &Sound::PlayingOffset_Set)
    });

    constructor = Napi::Persistent(tpl);
    constructor.SuppressDestruct();

    exports.Set("Sound", tpl);
}

Sound::Sound(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Sound>(info) {
    if(info.Length() < 1) {
        _sound = sfSound_create();
    } else {
        _sound = info[0].As<Napi::External<sfSound>>().Data();
    }
}

Sound::~Sound() {
    //sfSound_destroy(_sound);
}

Napi::Value Sound::Copy(const Napi::CallbackInfo& info) {
    sfSound* sfmlSound = sfSound_copy(_sound);
    Napi::External<sfSound> externalSound = Napi::External<sfSound>::New(info.Env(), sfmlSound);

    return constructor.New({ externalSound });
}

void Sound::Destroy(const Napi::CallbackInfo& info) {
    sfSound_destroy(_sound);
}

void Sound::Play(const Napi::CallbackInfo& info) {
    sfSound_play(_sound);
}

void Sound::Pause(const Napi::CallbackInfo& info) {
    sfSound_pause(_sound);
}

void Sound::Stop(const Napi::CallbackInfo& info) {
    sfSound_stop(_sound);
}

Napi::Value Sound::GetStatus(const Napi::CallbackInfo& info) {
    sfSoundStatus status = sfSound_getStatus(_sound);

    return Napi::Number::New(info.Env(), status);
}

Napi::Value Sound::Buffer_Get(const Napi::CallbackInfo& info) {
    sfSoundBuffer* sfmlSoundBuffer = const_cast<sfSoundBuffer*>(sfSound_getBuffer(_sound));
    Napi::External<sfSoundBuffer> externalSoundBuffer = Napi::External<sfSoundBuffer>::New(info.Env(), sfmlSoundBuffer);

    return SoundBuffer::constructor.New({ externalSoundBuffer });
}

void Sound::Buffer_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    Napi::Object obj = value.As<Napi::Object>();
    SoundBuffer* soundBuffer = SoundBuffer::Unwrap(obj);

    sfSound_setBuffer(_sound, soundBuffer->GetSoundBuffer());
}

Napi::Value Sound::Loop_Get(const Napi::CallbackInfo& info) {
    sfBool looping = sfSound_getLoop(_sound);

    return Napi::Boolean::New(info.Env(), looping != sfFalse);
}

void Sound::Loop_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    bool looping = value.As<Napi::Boolean>();

    sfSound_setLoop(_sound, looping ? sfTrue : sfFalse);
}

Napi::Value Sound::Pitch_Get(const Napi::CallbackInfo& info) {
    float pitch = sfSound_getPitch(_sound);

    return Napi::Number::New(info.Env(), pitch);
}

void Sound::Pitch_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    float pitch = value.As<Napi::Number>().FloatValue();

    sfSound_setPitch(_sound, pitch);
}

Napi::Value Sound::Volume_Get(const Napi::CallbackInfo& info) {
    float volume = sfSound_getVolume(_sound);

    return Napi::Number::New(info.Env(), volume);
}

void Sound::Volume_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    float volume = value.As<Napi::Number>().FloatValue();

    sfSound_setVolume(_sound, volume);
}

Napi::Value Sound::Position_Get(const Napi::CallbackInfo& info) {
    sfVector3f position = sfSound_getPosition(_sound);
    Napi::Object obj = Napi::Object::New(info.Env());

    obj.Set("x", Napi::Number::New(info.Env(), position.x));
    obj.Set("y", Napi::Number::New(info.Env(), position.y));
    obj.Set("z", Napi::Number::New(info.Env(), position.z));

    return obj;
}

void Sound::Position_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    Napi::Object obj = value.As<Napi::Object>();
    sfVector3f position{0,0,0};

    if(obj.Has("x")) {
        position.x = obj.Get("x").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("y")) {
        position.y = obj.Get("y").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("z")) {
        position.z = obj.Get("z").As<Napi::Number>().FloatValue();
    }

    sfSound_setPosition(_sound, position);
}

Napi::Value Sound::RelativeToListener_Get(const Napi::CallbackInfo& info) {
    sfBool relativeToListener = sfSound_isRelativeToListener(_sound);

    return Napi::Boolean::New(info.Env(), relativeToListener != sfFalse);
}

void Sound::RelativeToListener_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    bool relativeToListener = value.As<Napi::Boolean>();

    sfSound_setRelativeToListener(_sound, relativeToListener ? sfTrue : sfFalse);
}

Napi::Value Sound::MinDistance_Get(const Napi::CallbackInfo& info) {
    float minDistance = sfSound_getMinDistance(_sound);

    return Napi::Number::New(info.Env(), minDistance);
}

void Sound::MinDistance_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    float minDistance = value.As<Napi::Number>().FloatValue();

    sfSound_setMinDistance(_sound, minDistance);
}

Napi::Value Sound::Attenuation_Get(const Napi::CallbackInfo& info) {
    float attenuation = sfSound_getAttenuation(_sound);

    return Napi::Number::New(info.Env(), attenuation);
}

void Sound::Attenuation_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    float attenuation = value.As<Napi::Number>().FloatValue();

    sfSound_setAttenuation(_sound, attenuation);
}

Napi::Value Sound::PlayingOffset_Get(const Napi::CallbackInfo& info) {
    sfTime playingOffset = sfSound_getPlayingOffset(_sound);

    return Napi::Number::New(info.Env(), playingOffset.microseconds);
}

void Sound::PlayingOffset_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    int64_t playingOffset = value.As<Napi::Number>().Int64Value();
    sfTime sfmlPlayingOffset{ playingOffset };
    sfSound_setPlayingOffset(_sound, sfmlPlayingOffset);
}

const sfSound* Sound::GetSound() {
    return _sound;
}