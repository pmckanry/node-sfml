#include "event.h"

Napi::Value GetEventObject(Napi::Env env, sfEvent event) {
    switch(event.type) {
        case sfEvtClosed:
            return GetBasicEvent(env, "event_closed");
        case sfEvtResized:
            return GetBasicEvent(env, "event_resized");
        case sfEvtLostFocus:
            return GetBasicEvent(env, "event_lost_focus");
        case sfEvtGainedFocus:
            return GetBasicEvent(env, "event_gained_focus");
        case sfEvtTextEntered:
            return GetTextEvent(env, "event_text_entered", event.text);
        case sfEvtKeyPressed:
            return GetKeyEvent(env, "event_key_pressed", event.key);
        case sfEvtKeyReleased:
            return GetKeyEvent(env, "event_key_released", event.key);
        case sfEvtMouseWheelScrolled:
            return GetMouseWheelScrollEvent(env, "event_mouse_wheel_scroll", event.mouseWheelScroll);
        case sfEvtMouseButtonPressed:
            return GetMouseButtonEvent(env, "event_mouse_button_pressed", event.mouseButton);
        case sfEvtMouseButtonReleased:
            return GetMouseButtonEvent(env, "event_mouse_button_released", event.mouseButton);
        case sfEvtMouseMoved:
            return GetMouseMoveEvent(env, "event_mouse_move", event.mouseMove);
        case sfEvtMouseEntered:
            return GetBasicEvent(env, "event_mouse_entered");
        case sfEvtMouseLeft:
            return GetBasicEvent(env, "event_mouse_left");
        case sfEvtJoystickButtonPressed:
            return GetJoystickButtonEvent(env, "event_joystick_button_pressed", event.joystickButton);
        case sfEvtJoystickButtonReleased:
            return GetJoystickButtonEvent(env, "event_joystick_button_released", event.joystickButton);
        case sfEvtJoystickMoved:
            return GetJoystickMoveEvent(env, "event_joystick_moved", event.joystickMove);
        case sfEvtJoystickConnected:
            return GetJoystickConnectEvent(env, "event_joystick_connected", event.joystickConnect);
        case sfEvtJoystickDisconnected:
            return GetJoystickConnectEvent(env, "event_joystick_disconnected", event.joystickConnect);
        case sfEvtTouchBegan:
            return GetTouchEvent(env, "event_touch_began", event.touch);
        case sfEvtTouchMoved:
            return GetTouchEvent(env, "event_touch_moved", event.touch);
        case sfEvtTouchEnded:
            return GetTouchEvent(env, "event_touch_ended", event.touch);
        case sfEvtSensorChanged:
            return GetSensorEvent(env, "event_sensor", event.sensor);
        default:
            return GetBasicEvent(env, "event_unknown");
    }
}