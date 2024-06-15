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