#pragma once
#include <Utils.h>


//kKeyboard = 0, 
//kMouse=1, 
//kGamepad=2,

const std::array<int,3> SupportedDevices {0,1,2};
const std::array<std::string, 3> device_names{"Keyboard", "Mouse", "Gamepad"};

enum Purpose {
	kNone,

	// Dialogue
	kToggle,
	kZoomEnable,
	kZoomIn,
	kZoomOut,
};

struct Feature {
	//std::string name;
	bool enabled=false;
	bool instant=false;
	bool invert=false;
	bool revert=false;

	Purpose purpose = kNone;
    std::map<int, int> keymap;

    Feature(){
		for (int i = 0; i < SupportedDevices.size(); ++i) {
            keymap[i] = -1;
        }
	};
};

namespace Modules {

	namespace Dialogue {
		// Features
		inline Feature Toggle;
        inline Feature ZoomEnable;
		inline Feature ZoomIn;
		inline Feature ZoomOut;
		inline Feature AutoToggle;

		inline bool listen_gradual_zoom = false;
		inline bool listen_auto_zoom = true;
        inline bool zoom_enabled = false;

		void funcToggle();
        void funcZoom(int a_device, bool _in);

		Purpose GetPurpose(int a_device, int keyMask);

		void LoadFeatures();
	
	};

	namespace Combat {
		// Features
        inline Feature ToggleCombat;
        inline Feature ToggleWeapon;
        inline Feature ToggleBowDraw;
        inline Feature ToggleMagicWield;
        inline Feature ToggleMagicCast;
        inline Feature ToggleSneak;
		
		void LoadFeatures();
    };

    namespace Other {
        // Features
        inline Feature ToggleCellChangeExterior;
        inline Feature ToggleCellChangeInterior;
		inline Feature DisallowPOVSwitch;
		inline Feature FixZoom;
		//inline Feature FixZoomHardcore;

		void LoadFeatures();
	};
};


namespace Settings {
    void LoadDefaults();
	void LoadFromJson(std::string path);
	void LoadSettings();
};


namespace LogSettings {
    inline bool log_trace = true;
    inline bool log_info = true;
    inline bool log_warning = true;
    inline bool log_error = true;
};