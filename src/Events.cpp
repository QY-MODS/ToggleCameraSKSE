#include "Events.h"



void OurEventSink::HandleDialogueInputs(RE::InputEvent* const* evns) {
    if (!Utilities::Menu::IsOpen(RE::DialogueMenu::MENU_NAME)) return;
    for (RE::InputEvent* e = *evns; e; e = e->next) {
        if (!e) continue;
        if (e->eventType.get() != RE::INPUT_EVENT_TYPE::kButton) continue;
        _HandleDialogueInputs(e->AsButtonEvent());
    }
}

void OurEventSink::_HandleDialogueInputs(RE::ButtonEvent* a_event) {
    uint32_t keyMask = a_event->idCode;
    auto _device = a_event->GetDevice();
    // check if _device is supported
    if (std::find(SupportedDevices.begin(), SupportedDevices.end(), _device) == SupportedDevices.end()) return;

    logger::trace("Device: {}, KeyMask: {}", _device, keyMask);

    const auto purpose = Modules::Dialogue::GetPurpose(_device, keyMask);
    if (purpose == kNone) return;

    float duration = a_event->heldDownSecs;
    bool isPressed = a_event->value != 0 && duration >= 0;
    bool isReleased = a_event->value == 0 && duration != 0;
    bool _toggle = false;  // switch for 1st/3rd person

    if (isPressed) {
        if (purpose == kZoomEnable)
            Modules::Dialogue::zoom_enabled = true;
        else if (purpose == kZoomIn && (Modules::Dialogue::zoom_enabled || !Modules::Dialogue::ZoomEnable.enabled)) {
            Modules::Dialogue::funcZoom(_device, true);
        } else if (purpose == kZoomOut && (Modules::Dialogue::zoom_enabled || !Modules::Dialogue::ZoomEnable.enabled)) {
            Modules::Dialogue::funcZoom(_device, false);
        }
    } else if (isReleased) {
        if (purpose == kZoomEnable)
            Modules::Dialogue::zoom_enabled = false;
        else if (purpose == kToggle && Modules::Dialogue::Toggle.enabled)
            _toggle = true;
    }
    if (_toggle) {
        Modules::Dialogue::funcToggle();
        _toggle = false;
    }
}

RE::BSEventNotifyControl OurEventSink::ProcessEvent(const RE::MenuOpenCloseEvent* event,
                                                    RE::BSTEventSource<RE::MenuOpenCloseEvent>*) {
    if (!event) return RE::BSEventNotifyControl::kContinue;
    if (!Modules::Dialogue::listen_auto_zoom) return RE::BSEventNotifyControl::kContinue;
    if (event->menuName != RE::DialogueMenu::MENU_NAME) return RE::BSEventNotifyControl::kContinue;
    if (!Modules::Dialogue::AutoToggle.enabled) return RE::BSEventNotifyControl::kContinue;

    if (event->opening) {
        if (RE::PlayerCamera::GetSingleton()->IsInThirdPerson() && !Modules::Dialogue::AutoToggle.invert) {
            Modules::Dialogue::funcToggle();
        }
        else if (RE::PlayerCamera::GetSingleton()->IsInFirstPerson() && Modules::Dialogue::AutoToggle.invert) {
			Modules::Dialogue::funcToggle();
		}
    
    }
    else if (!event->opening) {
        if (RE::PlayerCamera::GetSingleton()->IsInFirstPerson() && !Modules::Dialogue::AutoToggle.invert) {
            Modules::Dialogue::funcToggle();
        } else if (RE::PlayerCamera::GetSingleton()->IsInThirdPerson() && Modules::Dialogue::AutoToggle.invert) {
            Modules::Dialogue::funcToggle();
        }

    } 
    
    return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl OurEventSink::ProcessEvent(RE::InputEvent* const* evns, RE::BSTEventSource<RE::InputEvent*>*) {
    if (!evns) return RE::BSEventNotifyControl::kContinue;
    if (!*evns) return RE::BSEventNotifyControl::kContinue;

    if (MCP::listen_key) {
        for (RE::InputEvent* e = *evns; e; e = e->next) {
            if (!e) continue;
            if (e->eventType.get() != RE::INPUT_EVENT_TYPE::kButton) continue;
            RE::ButtonEvent* a_event = e->AsButtonEvent();
            if (a_event->IsHeld()) continue;
            uint32_t keyMask = a_event->idCode;
            auto _device = a_event->GetDevice();
            if (std::find(SupportedDevices.begin(), SupportedDevices.end(), _device) == SupportedDevices.end()) continue;

            float duration = a_event->heldDownSecs;
            bool isPressed = a_event->value != 0 && duration >= 0;
            bool isReleased = a_event->value == 0 && duration != 0;

            if (isPressed) {
                MCP::listen_key = false;
                MCP::detected_device = _device;
                MCP::detected_key = keyMask;
				logger::info("Input Key Detection -> Device: {}, KeyMask: {}", device_names[_device], keyMask);
                break;
			}
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    HandleDialogueInputs(evns);

    return RE::BSEventNotifyControl::kContinue;
}