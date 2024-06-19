#pragma once

#include "Serialization.h"


namespace Dialogue {
    const uint8_t n_hooks = 1;
    const size_t trampoline_size = n_hooks * 14;

    namespace OnCameraUpdate {
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

	namespace OnActorUpdate {
        static void thunk(RE::Actor* a_actor, float a_zPos, RE::TESObjectCELL* a_cell);
		static inline REL::Relocation<decltype(thunk)> func;

        static bool __Killmove(RE::Actor* a_actor);
        static bool __WeaponDraw(RE::Actor* a_actor);
        static bool __BowDraw(RE::Actor* a_actor);
        static bool __MagicDraw(RE::Actor* a_actor);
        static bool __MagicCast(RE::Actor* a_actor);
	};

	void InstallHooks();
};

namespace Hooks {
    void Install();
};