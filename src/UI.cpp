#include "UI.h"

void __stdcall MCP::RenderLog() {
    // add checkboxes to filter log levels
    ImGui::Checkbox("Trace", &LogSettings::log_trace);
    ImGui::SameLine();
    ImGui::Checkbox("Info", &LogSettings::log_info);
    ImGui::SameLine();
    ImGui::Checkbox("Warning", &LogSettings::log_warning);
    ImGui::SameLine();
    ImGui::Checkbox("Error", &LogSettings::log_error);

    // if "Generate Log" button is pressed, read the log file
    if (ImGui::Button("Generate Log")) logLines = Utilities::ReadLogFile();

    // Display each line in a new ImGui::Text() element
    for (const auto& line : logLines) {
        if (line.find("trace") != std::string::npos && !LogSettings::log_trace) continue;
        if (line.find("info") != std::string::npos && !LogSettings::log_info) continue;
        if (line.find("warning") != std::string::npos && !LogSettings::log_warning) continue;
        if (line.find("error") != std::string::npos && !LogSettings::log_error) continue;
        ImGui::Text(line.c_str());
    }
}

void MCP::RenderDeviceKeyCombo(const std::string& title,const std::string& label,bool& enabled, int& selected_device, std::map<int, int>& keymap) {
    
    
    ImGui::Checkbox((label + ":##" + title).c_str(), &enabled);
    ImGui::SameLine();
    ImGui::SetCursorPosX(170);
    ImGui::Text("");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    if (ImGui::BeginCombo(("##device_combo" + title + label).c_str(), device_names[selected_device].c_str(),
                            ImGuiComboFlags_HeightSmall)) {
        for (int n = 0; n < device_names.size(); ++n) {
            const bool is_selected = (selected_device == n);
            if (ImGui::Selectable(device_names[n].c_str(), is_selected)) {
                selected_device = n;
            }
            if (is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();
    ImGui::Text("Key");

    ImGui::SameLine();
    ImGui::SetNextItemWidth(100);
        
    std::string combo_label = "##key_combo_" + std::to_string(selected_device) + label;  // Ensure unique ID for each combo
    if (ImGui::BeginCombo(combo_label.c_str(), std::to_string(keymap[selected_device]).c_str())) {
        for (int n = -1; n < 600; ++n) {
            const bool is_selected = keymap[selected_device] == n;
            if (ImGui::Selectable(std::to_string(n).c_str(), is_selected)) keymap[selected_device] = n;
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    
}

void MCP::Register(){
    SKSEMenuFramework::SetSection(Utilities::mod_name);
    SKSEMenuFramework::AddSectionItem("Settings", RenderSettings);
    //SKSEMenuFramework::AddSectionItem("Status", RenderStatus);
    SKSEMenuFramework::AddSectionItem("Log", RenderLog);
}

void __stdcall MCP::RenderSettings(){
    Dialogue::Render();
    Combat::Render();
};

void MCP::Dialogue::Render(){

    // Key Detection
    if (ImGui::Button("Start Key Detection")) {
        detected_key = -1;
        detected_device = -1;
        listen_key = true;
    }
    if (listen_key) {
		ImGui::SameLine();
		ImGui::Text("Listening for key press...");
	}
    if (detected_key >= 0) {
        ImGui::SameLine();
        ImGui::Text("Detected Key: %d, Device: %s", detected_key, device_names[detected_device].c_str());
    }
    // help marker
    ImGui::SameLine();
    HelpMarker("Click 'Start',close this menu and press a key to detect it. You can view the detected key here.");

    std::string title = "Dialogue";
    if (ImGui::CollapsingHeader((title + "##Settings").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
        RenderDeviceKeyCombo(title,"Toggle",Modules::Dialogue::Toggle.enabled,Dialogue::Toggle::selected_device,Modules::Dialogue::Toggle.keymap);
        ImGui::SameLine();
        ImGui::Checkbox((std::string("InstantZoom") + "##" + title).c_str(), &Modules::Dialogue::Toggle.instant);
        RenderDeviceKeyCombo(title, "ZoomEnabler", Modules::Dialogue::ZoomEnable.enabled,Dialogue::Toggle::selected_device, Modules::Dialogue::ZoomEnable.keymap);
        RenderDeviceKeyCombo(title, "ZoomIn", Modules::Dialogue::ZoomIn.enabled, Dialogue::Toggle::selected_device,Modules::Dialogue::ZoomIn.keymap);
        RenderDeviceKeyCombo(title, "ZoomOut", Modules::Dialogue::ZoomOut.enabled, Dialogue::Toggle::selected_device,
                             Modules::Dialogue::ZoomOut.keymap);
        ImGui::Checkbox((std::string("AutoToggle") + "##" + title).c_str(), &Modules::Dialogue::AutoToggle.enabled);
        ImGui::SameLine();
        ImGui::SetCursorPosX(170);
        ImGui::Text("");
        ImGui::SameLine();
        ImGui::Checkbox((std::string("Invert") + "##" + title).c_str(), &Modules::Dialogue::AutoToggle.invert);
    }
    
};

void MCP::Combat::Render() {

    std::string title = "Combat";
    if (ImGui::CollapsingHeader((title + "##Settings").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
        __Render(ToggleCombat.enabled, ToggleCombat.invert, ToggleCombat.revert,ToggleCombat.instant, title, "ToggleCombatEnter");
        __Render(ToggleWeapon.enabled, ToggleWeapon.invert, ToggleWeapon.revert,ToggleWeapon.instant, title, "ToggleWeaponDraw");
        __Render(ToggleBowDraw.enabled, ToggleBowDraw.invert, ToggleBowDraw.revert,ToggleBowDraw.instant, title, "ToggleBowDraw");
        __Render(ToggleMagicWield.enabled, ToggleMagicWield.invert, ToggleMagicWield.revert, ToggleMagicWield.instant,title,"ToggleMagicWield");
        __Render(ToggleMagicCast.enabled, ToggleMagicCast.invert, ToggleMagicCast.revert, ToggleMagicCast.instant,title,"ToggleMagicCast");

    }
}

void MCP::Combat::__Render(bool& enabled, bool& invert, bool& revert, bool& instant, const std::string& title,
                           const std::string& label) {
    ImGui::Checkbox((label + "##" + title).c_str(), &enabled);
    ImGui::SameLine();
    ImGui::SetCursorPosX(270);
    ImGui::Text("");
    ImGui::SameLine();
    ImGui::Checkbox((std::string("Invert") + "##" + title+label).c_str(), &invert);
    ImGui::SameLine();
    ImGui::Checkbox((std::string("Revert") + "##" + title + label).c_str(), &revert);
    ImGui::SameLine();
    ImGui::Checkbox((std::string("Instant") + "##" + title + label).c_str(), &instant);
}
