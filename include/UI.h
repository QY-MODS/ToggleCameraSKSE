#pragma once

#include "Hooks.h"


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

    void RenderDeviceKeyCombo(const std::string& title,const std::string& label,bool& enabled, int& selected_device, std::map<int, int>& keymap);
    
    namespace Dialogue {
        void Render();


        namespace Toggle{
            inline int selected_device = 0;
        };
	};
    
};