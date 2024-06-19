#pragma once
// #include <chrono>
#include <windows.h>

#include <algorithm>
#include <codecvt>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "SimpleIni.h"
#include "rapidjson/document.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "SKSEMenuFramework.h"
#include <ClibUtil/editorID.hpp>


static void HelpMarker(const char* desc) {
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip()) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

static ImGuiTableFlags table_flags = ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;

namespace Utilities {

    const auto mod_name = static_cast<std::string>(SKSE::PluginDeclaration::GetSingleton()->GetName());

    std::filesystem::path GetLogPath();

    std::vector<std::string> ReadLogFile();

    namespace Menu {

        bool IsOpen(const std::string_view menuname);

    };

    bool IsMagicEquipped();

    bool IsCasting();

    inline uint32_t GetCombatState() { return RE::PlayerCharacter::GetSingleton()->IsInCombat(); }

};