#include "music.h"

Napi::FunctionReference Music::constructor;

void Music::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function tpl = DefineClass(env, "Music", {
        InstanceMethod("destroy", &Music::Destroy),
        InstanceMethod("getDuration", &Music::GetDuration),
        InstanceMethod("play", &Music::Play),
        InstanceMethod("pause", &Music::Pause),
        InstanceMethod("stop", &Music::Stop),
        InstanceMethod("getChannelCount", &Music::GetChannelCount),
        InstanceMethod("getSampleRate", &Music::GetSampleRate),
        InstanceMethod("getStatus", &Music::GetStatus),
        InstanceAccessor("loop", &Music::Loop_Get, &Music::Loop_Set),
        InstanceAccessor("playingOffset", &Music::PlayingOffset_Get, &Music::PlayingOffset_Set),
        InstanceAccessor("pitch", &Music::Pitch_Get, &Music::Pitch_Set),
        InstanceAccessor("volume", &Music::Volume_Get, &Music::Volume_Set),
        InstanceAccessor("position", &Music::Position_Get, &Music::Position_Set),
        InstanceAccessor("relativeToListener", &Music::RelativeToListener_Get, &Music::RelativeToListener_Set),
        InstanceAccessor("minDistance", &Music::MinDistance_Get, &Music::MinDistance_Set),
        InstanceAccessor("attenuation", &Music::Attenuation_Get, &Music::Attenuation_Set),
        StaticMethod("createFromMemory", &Music::CreateFromMemory)
    });

    constructor = Napi::Persistent(tpl);
    constructor.SuppressDestruct();

    exports.Set("Music", tpl);
}

Music::Music(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Music>(info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }
    if(info[0].IsString()) {
        std::string filename = info[0].As<Napi::String>();

        _music = sfMusic_createFromFile(filename.c_str());
    } else {
        _music = info[0].As<Napi::External<sfMusic>>().Data();
    }
}

Music::~Music() {
    //sfMusic_destroy(_music);
}

// Methods
void Music::Destroy(const Napi::CallbackInfo& info) {
    sfMusic_destroy(_music);
}

Napi::Value Music::GetDuration(const Napi::CallbackInfo& info) {
    sfTime duration = sfMusic_getDuration(_music);

    return Napi::Number::New(info.Env(), duration.microseconds);
}

void Music::Play(const Napi::CallbackInfo& info) {
    sfMusic_play(_music);
}

void Music::Pause(const Napi::CallbackInfo& info) {
    sfMusic_pause(_music);
}

void Music::Stop(const Napi::CallbackInfo& info) {
    sfMusic_stop(_music);
}

Napi::Value Music::GetChannelCount(const Napi::CallbackInfo& info) {
    int channelCount = sfMusic_getChannelCount(_music);

    return Napi::Number::New(info.Env(), channelCount);
}

Napi::Value Music::GetSampleRate(const Napi::CallbackInfo& info) {
    uint32_t sampleRate = sfMusic_getSampleRate(_music);

    return Napi::Number::New(info.Env(), sampleRate);
}

Napi::Value Music::GetStatus(const Napi::CallbackInfo& info) {
    sfSoundStatus status = sfMusic_getStatus(_music);

    return Napi::Number::New(info.Env(), status);
}

// Accessors
Napi::Value Music::Loop_Get(const Napi::CallbackInfo& info) {
    sfBool looping = sfMusic_getLoop(_music);

    return Napi::Boolean::New(info.Env(), looping != sfFalse);
}

void Music::Loop_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    bool looping = value.As<Napi::Boolean>();

    sfMusic_setLoop(_music, looping ? sfTrue : sfFalse);
}

Napi::Value Music::PlayingOffset_Get(const Napi::CallbackInfo& info) {
    sfTime playingOffset = sfMusic_getPlayingOffset(_music);

    return Napi::Number::New(info.Env(), playingOffset.microseconds);
}

void Music::PlayingOffset_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    int64_t playingOffset = value.As<Napi::Number>().Int64Value();
    sfTime sfmlPlayingOffset { playingOffset };

    sfMusic_setPlayingOffset(_music, sfmlPlayingOffset);
}

Napi::Value Music::Pitch_Get(const Napi::CallbackInfo& info) {
    float pitch = sfMusic_getPitch(_music);

    return Napi::Number::New(info.Env(), pitch);
}

void Music::Pitch_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    float pitch = value.As<Napi::Number>().FloatValue();

    sfMusic_setPitch(_music, pitch);
}

Napi::Value Music::Volume_Get(const Napi::CallbackInfo& info) {
    float volume = sfMusic_getVolume(_music);

    return Napi::Number::New(info.Env(), volume);
}

void Music::Volume_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    float volume = value.As<Napi::Number>().FloatValue();

    sfMusic_setVolume(_music, volume);
}

Napi::Value Music::Position_Get(const Napi::CallbackInfo& info) {
    sfVector3f position = sfMusic_getPosition(_music);
    Napi::Object obj = Napi::Object::New(info.Env());

    obj.Set("x", Napi::Number::New(info.Env(), position.x));
    obj.Set("y", Napi::Number::New(info.Env(), position.y));
    obj.Set("z", Napi::Number::New(info.Env(), position.z));

    return obj;
}

void Music::Position_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    Napi::Object obj = value.As<Napi::Object>();
    sfVector3f position {0,0,0};

    if(obj.Has("x")) {
        position.x = obj.Get("x").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("y")) {
        position.y = obj.Get("y").As<Napi::Number>().FloatValue();
    }
    if(obj.Has("z")) {
        position.z = obj.Get("z").As<Napi::Number>().FloatValue();
    }

    sfMusic_setPosition(_music, position);
}

Napi::Value Music::RelativeToListener_Get(const Napi::CallbackInfo& info) {
    sfBool relativeToListener = sfMusic_isRelativeToListener(_music);

    return Napi::Boolean::New(info.Env(), relativeToListener != sfFalse);
}

void Music::RelativeToListener_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    bool relativeToListener = value.As<Napi::Boolean>();

    sfMusic_setRelativeToListener(_music, relativeToListener ? sfTrue : sfFalse);
}

Napi::Value Music::MinDistance_Get(const Napi::CallbackInfo& info) {
    float minDistance = sfMusic_getMinDistance(_music);

    return Napi::Number::New(info.Env(), minDistance);
}

void Music::MinDistance_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    float minDistance = value.As<Napi::Number>().FloatValue();

    sfMusic_setMinDistance(_music, minDistance);
}

Napi::Value Music::Attenuation_Get(const Napi::CallbackInfo& info) {
    float attenuation = sfMusic_getAttenuation(_music);

    return Napi::Number::New(info.Env(), attenuation);
}

void Music::Attenuation_Set(const Napi::CallbackInfo& info, const Napi::Value& value) {
    float attenuation = value.As<Napi::Number>().FloatValue();

    sfMusic_setAttenuation(_music, attenuation);
}

Napi::Value Music::CreateFromMemory(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }
    Napi::Buffer<void*> musicBuffer = info[0].As<Napi::Buffer<void*>>();

    sfMusic* sfmlMusic = sfMusic_createFromMemory(musicBuffer.Data(), musicBuffer.Length());
    Napi::External<sfMusic> externalMusic = Napi::External<sfMusic>::New(info.Env(), sfmlMusic);

    return constructor.New({ externalMusic });
}

/*
Napi::Value Music::CreateFromStream(const Napi::CallbackInfo& info) {

}
*/

const sfMusic* Music::GetMusic() {
    return _music;
}