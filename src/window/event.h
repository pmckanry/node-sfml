#ifndef SFML_NODE_WINDOW_EVENT_H
#define SFML_NODE_WINDOW_EVENT_H

#include <napi.h>
#include <SFML/Window/Event.h>

Napi::Value GetEventObject(Napi::Env env, sfEvent event);

inline Napi::Value GetBasicEvent(Napi::Env env, const char* eventName) {
    Napi::Object obj = Napi::Object::New(env);
    
    obj.Set("type", Napi::String::New(env, eventName));

    return obj;
}

inline Napi::Value GetSizeEvent(Napi::Env env, const char* eventName, sfSizeEvent event) {
    Napi::Object obj = Napi::Object::New(env);
    
    obj.Set("type", Napi::String::New(env, eventName));
    obj.Set("width", Napi::Number::New(env, event.width));
    obj.Set("height", Napi::Number::New(env, event.height));

    return obj;
}

inline Napi::Value GetKeyEvent(Napi::Env env, const char* eventName, sfKeyEvent event) {
    Napi::Object obj = Napi::Object::New(env);
    
    obj.Set("type", Napi::String::New(env, eventName));
    obj.Set("keyCode", Napi::Number::New(env, event.code));
    obj.Set("alt", Napi::Boolean::New(env, event.alt));
    obj.Set("control", Napi::Boolean::New(env, event.control));
    obj.Set("shift", Napi::Boolean::New(env, event.shift));
    obj.Set("system", Napi::Boolean::New(env, event.system));

    return obj;
}

inline Napi::Value GetTextEvent(Napi::Env env, const char* eventName, sfTextEvent event) {
    Napi::Object obj = Napi::Object::New(env);
    
    obj.Set("type", Napi::String::New(env, eventName));
    obj.Set("unicode", Napi::Number::New(env, event.unicode));

    return obj;
}

inline Napi::Value GetMouseMoveEvent(Napi::Env env, const char* eventName, sfMouseMoveEvent event) {
    Napi::Object obj = Napi::Object::New(env);
    
    obj.Set("type", Napi::String::New(env, eventName));
    obj.Set("x", Napi::Number::New(env, event.x));
    obj.Set("y", Napi::Number::New(env, event.y));

    return obj;
}

inline Napi::Value GetMouseButtonEvent(Napi::Env env, const char* eventName, sfMouseButtonEvent event) {
    Napi::Object obj = Napi::Object::New(env);
    
    obj.Set("type", Napi::String::New(env, eventName));
    obj.Set("x", Napi::Number::New(env, event.x));
    obj.Set("y", Napi::Number::New(env, event.y));
    obj.Set("button", Napi::Number::New(env, event.button));

    return obj;
}

inline Napi::Value GetMouseWheelScrollEvent(Napi::Env env, const char* eventName, sfMouseWheelScrollEvent event) {
    Napi::Object obj = Napi::Object::New(env);
    
    obj.Set("type", Napi::String::New(env, eventName));
    obj.Set("x", Napi::Number::New(env, event.x));
    obj.Set("y", Napi::Number::New(env, event.y));
    obj.Set("wheel", Napi::Number::New(env, event.wheel));
    obj.Set("delta", Napi::Number::New(env, event.delta));

    return obj;
}

inline Napi::Value GetJoystickMoveEvent(Napi::Env env, const char* eventName, sfJoystickMoveEvent event) {
    Napi::Object obj = Napi::Object::New(env);
    
    obj.Set("type", Napi::String::New(env, eventName));
    obj.Set("id", Napi::Number::New(env, event.joystickId));
    obj.Set("axis", Napi::Number::New(env, event.axis));
    obj.Set("position", Napi::Number::New(env, event.position));

    return obj;
}

inline Napi::Value GetJoystickButtonEvent(Napi::Env env, const char* eventName, sfJoystickButtonEvent event) {
    Napi::Object obj = Napi::Object::New(env);
    
    obj.Set("type", Napi::String::New(env, eventName));
    obj.Set("id", Napi::Number::New(env, event.joystickId));
    obj.Set("button", Napi::Number::New(env, event.button));

    return obj;
}

inline Napi::Value GetJoystickConnectEvent(Napi::Env env, const char* eventName, sfJoystickConnectEvent event) {
    Napi::Object obj = Napi::Object::New(env);
    
    obj.Set("type", Napi::String::New(env, eventName));
    obj.Set("id", Napi::Number::New(env, event.joystickId));

    return obj;
}

inline Napi::Value GetTouchEvent(Napi::Env env, const char* eventName, sfTouchEvent event) {
    Napi::Object obj = Napi::Object::New(env);
    
    obj.Set("type", Napi::String::New(env, eventName));
    obj.Set("finger", Napi::Number::New(env, event.finger));
    obj.Set("x", Napi::Number::New(env, event.x));
    obj.Set("y", Napi::Number::New(env, event.y));

    return obj;
}

inline Napi::Value GetSensorEvent(Napi::Env env, const char* eventName, sfSensorEvent event) {
    Napi::Object obj = Napi::Object::New(env);
    
    obj.Set("type", Napi::String::New(env, eventName));
    obj.Set("sensor", Napi::Number::New(env, event.sensorType));
    obj.Set("x", Napi::Number::New(env, event.x));
    obj.Set("y", Napi::Number::New(env, event.y));
    obj.Set("z", Napi::Number::New(env, event.z));

    return obj;
}

#endif // SFML_NODE_WINDOW_EVENT_H