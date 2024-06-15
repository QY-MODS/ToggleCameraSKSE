#include "Hooks.h"


void Dialogue::OnCameraUpdate::thunk(RE::TESCamera* a_camera){
	func(a_camera);
    if (!Modules::Dialogue::listen_gradual_zoom) return;
    if (auto* thirdPersonState = static_cast<RE::ThirdPersonState*>(a_camera->currentState.get());
        thirdPersonState &&
        thirdPersonState->currentZoomOffset < -0.19f) {
        Modules::Dialogue::listen_gradual_zoom = false;
        RE::PlayerCamera::GetSingleton()->ForceFirstPerson();
        Modules::Dialogue::listen_auto_zoom = true;
    }
	
};


void Dialogue::InstallHooks() {
    
    auto& trampoline = SKSE::GetTrampoline();

    REL::Relocation<std::uintptr_t> camFunction{REL::RelocationID(49852, 50784)};
    OnCameraUpdate::func = trampoline.write_call<5>(camFunction.address() + REL::Relocate(0x1A6, 0x1A6), OnCameraUpdate::thunk);

};

void Hooks::Install(){

    // currently only hook is dialogue
    auto& trampoline = SKSE::GetTrampoline();
    trampoline.create(Dialogue::trampoline_size);

    Dialogue::InstallHooks();
};
