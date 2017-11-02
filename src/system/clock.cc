#include "clock.h"

Napi::FunctionReference Clock::constructor;

void Clock::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function tpl = DefineClass(env, "Clock", {
        InstanceMethod("copy", &Clock::Copy),
        InstanceMethod("getElapsedTime", &Clock::GetElapsedTime),
        InstanceMethod("restart", &Clock::Restart)
    });

    constructor = Napi::Persistent(tpl);
    constructor.SuppressDestruct();

    exports.Set("Clock", tpl);
}

Clock::Clock(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Clock>(info) {
    // Passed in a sfClock
    if(info.Length() > 0) {
        _clock = info[0].As<Napi::External<sfClock>>().Data();
    } else {
        _clock = sfClock_create();
    }
}

Clock::~Clock() {
    sfClock_destroy(_clock);
}

Napi::Value Clock::Copy(const Napi::CallbackInfo& info) {
    sfClock* sfmlClock = sfClock_copy(_clock);

    Napi::External<sfClock> externalClock = Napi::External<sfClock>::New(info.Env(), sfmlClock);

    return constructor.New({ externalClock });
}

Napi::Value Clock::GetElapsedTime(const Napi::CallbackInfo& info) {
    sfTime time = sfClock_getElapsedTime(_clock);

    return Napi::Number::New(info.Env(), time.microseconds);
}

Napi::Value Clock::Restart(const Napi::CallbackInfo& info) {
    sfTime time = sfClock_restart(_clock);
    
    return Napi::Number::New(info.Env(), time.microseconds);
}