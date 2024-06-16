#pragma once

#include "Settings.h"



namespace Dialogue {
    const uint8_t n_hooks = 1;
    const size_t trampoline_size = n_hooks * 14;

    struct OnCameraUpdate {
        static void thunk(RE::TESCamera* a_camera);
        static inline REL::Relocation<decltype(thunk)> func;
    };

    void InstallHooks();
};

namespace Combat {
    using namespace Modules::Combat;
    using namespace Utilities;

	const uint8_t n_hooks = 1;
	const size_t trampoline_size = n_hooks * 14;

	struct OnActorUpdate {
        static void thunk(RE::Actor* a_actor, float a_zPos, RE::TESObjectCELL* a_cell);
		static inline REL::Relocation<decltype(thunk)> func;
	};

	void InstallHooks();
};

namespace Hooks {
    void Install();
};