#ifndef SFML_NODE_SYSTEM_CLOCK_H
#define SFML_NODE_SYSTEM_CLOCK_H

#include <napi.h>

#include <SFML/System/Clock.h>

class Clock : public Napi::ObjectWrap<Clock> {
public:
    static Napi::FunctionReference constructor;
    static void Init(Napi::Env env, Napi::Object exports);

    Clock(const Napi::CallbackInfo& info);
    ~Clock();

    Napi::Value Copy(const Napi::CallbackInfo& info);
    Napi::Value GetElapsedTime(const Napi::CallbackInfo& info);
    Napi::Value Restart(const Napi::CallbackInfo& info);

private:
    sfClock* _clock;
};

#endif // SFML_NODE_SYSTEM_CLOCK_H