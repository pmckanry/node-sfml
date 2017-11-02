#include "soundbuffer.h"

Napi::FunctionReference SoundBuffer::constructor;

void SoundBuffer::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function tpl = DefineClass(env, "SoundBuffer", {
        InstanceMethod("copy", &SoundBuffer::Copy),
        InstanceMethod("destroy", &SoundBuffer::Destroy),
        InstanceMethod("saveToFile", &SoundBuffer::SaveToFile),
        InstanceMethod("getSamples", &SoundBuffer::GetSamples),
        InstanceMethod("getSampleCount", &SoundBuffer::GetSampleCount),
        InstanceMethod("getSampleRate", &SoundBuffer::GetSampleRate),
        InstanceMethod("getChannelCount", &SoundBuffer::GetChannelCount),
        InstanceMethod("getDuration", &SoundBuffer::GetDuration),
        StaticMethod("createFromMemory", &SoundBuffer::CreateFromMemory),
        StaticMethod("createFromSamples", &SoundBuffer::CreateFromSamples)
    });

    constructor = Napi::Persistent(tpl);
    constructor.SuppressDestruct();

    exports.Set("SoundBuffer", tpl);
}

SoundBuffer::SoundBuffer(const Napi::CallbackInfo& info) : Napi::ObjectWrap<SoundBuffer>(info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }
    if(info[0].IsString()) {
        std::string filename = info[0].As<Napi::String>().Utf8Value();
        _soundBuffer = sfSoundBuffer_createFromFile(filename.c_str());
    } else {
        _soundBuffer = info[0].As<Napi::External<sfSoundBuffer>>().Data();
    }
}

SoundBuffer::~SoundBuffer() {
    //sfSoundBuffer_destroy(_soundBuffer);
}

// Methods
Napi::Value SoundBuffer::Copy(const Napi::CallbackInfo& info) {
    sfSoundBuffer* sfmlSoundBuffer = sfSoundBuffer_copy(_soundBuffer);
    Napi::External<sfSoundBuffer> externalSoundBuffer = Napi::External<sfSoundBuffer>::New(info.Env(), sfmlSoundBuffer);

    return constructor.New({ externalSoundBuffer });
}

void SoundBuffer::Destroy(const Napi::CallbackInfo& info) {
    sfSoundBuffer_destroy(_soundBuffer);
}

Napi::Value SoundBuffer::SaveToFile(const Napi::CallbackInfo& info) {
    if(info.Length() < 1) {
        throw Napi::Error::New(info.Env(), "Invalid number of arguments");
    }
    
    std::string filename = info[0].As<Napi::String>().Utf8Value();
    sfBool wasSuccessful = sfSoundBuffer_saveToFile(_soundBuffer, filename.c_str());

    return Napi::Boolean::New(info.Env(), wasSuccessful != sfFalse);
}

Napi::Value SoundBuffer::GetSamples(const Napi::CallbackInfo& info) {
    const sfInt16* samples = sfSoundBuffer_getSamples(_soundBuffer);
    sfInt64 sampleCount = sfSoundBuffer_getSampleCount(_soundBuffer);

    Napi::Array arr = Napi::Array::New(info.Env(), sampleCount);

    for(sfInt64 i = 0; i < sampleCount; i++) {
        arr.Set(i, Napi::Number::New(info.Env(), samples[i]));
    }

    return arr;
}

Napi::Value SoundBuffer::GetSampleCount(const Napi::CallbackInfo& info) {
    sfInt64 sampleCount = sfSoundBuffer_getSampleCount(_soundBuffer);

    return Napi::Number::New(info.Env(), sampleCount);
}

Napi::Value SoundBuffer::GetSampleRate(const Napi::CallbackInfo& info) {
    uint32_t sampleRate = sfSoundBuffer_getSampleRate(_soundBuffer);

    return Napi::Number::New(info.Env(), sampleRate);
}

Napi::Value SoundBuffer::GetChannelCount(const Napi::CallbackInfo& info) {
    uint32_t channelCount = sfSoundBuffer_getChannelCount(_soundBuffer);

    return Napi::Number::New(info.Env(), channelCount);
}

Napi::Value SoundBuffer::GetDuration(const Napi::CallbackInfo& info) {
    sfTime duration = sfSoundBuffer_getDuration(_soundBuffer);

    return Napi::Number::New(info.Env(), duration.microseconds);
}

// Static
Napi::Value SoundBuffer::CreateFromMemory(const Napi::CallbackInfo& info) {
    return info.Env().Undefined();
}

Napi::Value SoundBuffer::CreateFromStream(const Napi::CallbackInfo& info) {
    return info.Env().Undefined();
}

Napi::Value SoundBuffer::CreateFromSamples(const Napi::CallbackInfo& info) {
    return info.Env().Undefined();
}


const sfSoundBuffer* SoundBuffer::GetSoundBuffer() {
    return _soundBuffer;
}