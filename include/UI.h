#pragma once

#include "Hooks.h"

void HelpMarker(const char* desc);

const ImGuiTableFlags table_flags =
    ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;

namespace MCP {

    inline std::string log_path = Utilities::GetLogPath().string();
    inline std::vector<std::string> logLines;

    inline int detected_key = -1;
    inline int detected_device = -1;
    inline bool listen_key = false;

    void Register();

    void __stdcall RenderSettings();
    //void __stdcall RenderStatus();
    void __stdcall RenderLog();

    void RenderCheckBox(const std::string& title, const std::string& label, bool& enabled);
    void RenderDeviceKeyCombo(const std::string& title,const std::string& label,bool& enabled, int& selected_device, std::map<int, int>& keymap);
    void RenderZoomLvL(const std::string& title, const std::string& label, Feature& feat);

    namespace Dialogue {
        void Render();
        namespace Toggle{
            inline int selected_device = 0;
        };
        void RenderEnableDisableAll();
	};
    
    namespace Combat {
        using namespace Modules::Combat;
        void Render();
        void __Render(Feature& feat, const std::string& title, const std::string& label);
        void RenderEnableDisableAll();
    };

    namespace Other {
        using namespace Modules::Other;
    	void Render();
        void __Render(bool& enabled, bool& invert, const std::string& title,
					  const std::string& label);
		void RenderEnableDisableAll();
    };
};