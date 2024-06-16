#include "Settings.h"

void Settings::LoadDefaults(){
	Modules::Dialogue::LoadFeatures();
    Modules::Combat::LoadFeatures();
    Modules::Other::LoadFeatures();
}
void Settings::LoadSettings(){
    LoadDefaults();
	//LoadFromJson("Data/SKSE/Plugins/DialogueCam.json");
};

void Modules::Dialogue::funcToggle() {
    listen_gradual_zoom = false;
    listen_auto_zoom = false;

    auto plyr_c = RE::PlayerCamera::GetSingleton();
    auto thirdPersonState =
        static_cast<RE::ThirdPersonState*>(plyr_c->cameraStates[RE::CameraState::kThirdPerson].get());
    if (!thirdPersonState) {
		logger::error("ThirdPersonState is null.");
		return;
	}
    if (plyr_c->IsInFirstPerson()) {
        plyr_c->ForceThirdPerson();
        thirdPersonState->targetZoomOffset = thirdPersonState->savedZoomOffset;
    } else if (plyr_c->IsInThirdPerson()) {
        thirdPersonState->savedZoomOffset = thirdPersonState->currentZoomOffset;
        if (!Toggle.instant) {
            listen_gradual_zoom = true;
            thirdPersonState->targetZoomOffset = -0.2f;
            return;
        }
        plyr_c->ForceFirstPerson();
    } else logger::error("Player is in neither 1st nor 3rd person.");

    listen_auto_zoom = true;
}

void Modules::Dialogue::funcZoom(int a_device, bool _in) {
    listen_gradual_zoom = false;
    listen_auto_zoom = false;

    auto player_cam = RE::PlayerCamera::GetSingleton();
    auto is_in_first = player_cam->IsInFirstPerson();

    auto thirdPersonState =
        static_cast<RE::ThirdPersonState*>(player_cam->cameraStates[RE::CameraState::kThirdPerson].get());
    if (!thirdPersonState) {
        logger::error("ThirdPersonState is null.");
        return;
    }
    float amount = (a_device % 2) ? 0.1f : 0.025f;
    if (_in) {
        if (is_in_first);
        else if (thirdPersonState->currentZoomOffset < -0.19f) player_cam->ForceFirstPerson();
        else thirdPersonState->targetZoomOffset = std::max(thirdPersonState->targetZoomOffset - amount, -0.2f);
    } 
    else if (is_in_first) player_cam->ForceThirdPerson();
    else thirdPersonState->targetZoomOffset = std::min(thirdPersonState->targetZoomOffset + amount, 1.0f);

    listen_auto_zoom = true;
}

Purpose Modules::Dialogue::GetPurpose(int a_device, int keyMask) {
    const auto failed = kNone;
    for (const auto& device : SupportedDevices) {
        if (a_device != device) continue;
        if (keyMask == Toggle.keymap[device]) return kToggle;
        if (keyMask == ZoomEnable.keymap[device]) return kZoomEnable;
        if (keyMask == ZoomIn.keymap[device]) return kZoomIn;
        if (keyMask == ZoomOut.keymap[device]) return kZoomOut;
    }
    return failed;
}

void Modules::Dialogue::LoadFeatures(){
	Toggle.enabled = true;
	Toggle.instant = false;
    
    ZoomEnable.enabled = true;
	ZoomIn.enabled = true;
	ZoomOut.enabled = true;

    Toggle.keymap = {{0, 33}, {1, -1}, {2, 128}};
    ZoomEnable.keymap = {{0, 29}, {1, -1}, {2, 64}};
    ZoomIn.keymap = {{0, -1}, {1, 8}, {2, 10}};
    ZoomOut.keymap = {{0, -1}, {1, 9}, {2, 512}};
};


bool Modules::Combat::Is3rdP() {
    auto plyr_c = RE::PlayerCamera::GetSingleton();
    if (!plyr_c) {
    	logger::error("PlayerCamera is null.");
		return false;
    }
    if (plyr_c->IsInFirstPerson()) return false;
    auto thirdPersonState =
        static_cast<RE::ThirdPersonState*>(plyr_c->cameraStates[RE::CameraState::kThirdPerson].get());
    if (thirdPersonState->targetZoomOffset != thirdPersonState->currentZoomOffset &&
        thirdPersonState->targetZoomOffset == -0.2f && listen_gradual_zoom) {
        return false;
    }
    else return plyr_c->IsInThirdPerson();
}

void Modules::Combat::funcToggle(bool gradual, float extra_offset) {
    auto plyr_c = RE::PlayerCamera::GetSingleton();
    if (!plyr_c) {
		logger::error("PlayerCamera is null.");
        return;
    }
    bool is3rdP = Is3rdP();
    listen_gradual_zoom = false;
    auto thirdPersonState =
        static_cast<RE::ThirdPersonState*>(plyr_c->cameraStates[RE::CameraState::kThirdPerson].get());
    if (!thirdPersonState) {
        logger::error("ThirdPersonState is null.");
        return;
    }
    if (!is3rdP) {
        plyr_c->ForceThirdPerson();
        thirdPersonState->targetZoomOffset = savedZoomOffset + extra_offset;
    } 
    else if (gradual) {
        listen_gradual_zoom = true;
        thirdPersonState->targetZoomOffset = -0.2f;
    } 
    else plyr_c->ForceFirstPerson();
}

uint32_t Modules::Combat::CamSwitchHandling(uint32_t newstate, bool third2first, bool switch_back) { 
    // Toggle i call lamali miyiz ona bakiyoruz
    const bool is_3rd_p = Is3rdP();
    bool player_is_in_toggled_cam = third2first ? !is_3rd_p : is_3rd_p;

    if (newstate) {
        if (player_is_in_toggled_cam) {
            return 0;
        }
    } else {
        if (!player_is_in_toggled_cam) {
            return 0;
        } else if (!switch_back) {
            return 0;
        }
    }
    return 1;
}

void Modules::Combat::LoadFeatures() {
    ToggleCombat.enabled = true;
    ToggleWeapon.enabled = true;
    ToggleBowDraw.enabled = true;
    ToggleMagicWield.enabled = true;
    ToggleMagicCast.enabled = true;
    ToggleSneak.enabled = true;
}

void Modules::Other::LoadFeatures() {
    ToggleCellChangeInterior.enabled = false;
    ToggleCellChangeInterior.instant = false;
    
    ToggleCellChangeExterior.enabled = false;
    ToggleCellChangeExterior.instant = false;

    DisallowPOVSwitch.enabled = false;
    FixZoom.enabled = false;
}