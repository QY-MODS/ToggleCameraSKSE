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
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/error/en.h>
#include <rapidjson/writer.h>
#include "SKSEMCP/SKSEMenuFramework.hpp"
#include <ClibUtil/editorID.hpp>


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

    inline std::string formatFloatToString(float value, int precision) {
        std::ostringstream out;
        out << std::fixed << std::setprecision(precision) << value;
        return out.str();
    }
};