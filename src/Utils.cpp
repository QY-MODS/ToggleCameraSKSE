#include "Utils.h"

bool Utilities::Menu::IsOpen(const std::string_view menuname) {
    if (auto ui = RE::UI::GetSingleton()) {
        if (ui->IsMenuOpen(menuname)) return true;
    }
    return false;
}

std::filesystem::path Utilities::GetLogPath() {
    auto logsFolder = SKSE::log::log_directory();
    if (!logsFolder) SKSE::stl::report_and_fail("SKSE log_directory not provided, logs disabled.");
    auto pluginName = SKSE::PluginDeclaration::GetSingleton()->GetName();
    auto logFilePath = *logsFolder / std::format("{}.log", pluginName);
    return logFilePath;
}

std::vector<std::string> Utilities::ReadLogFile() {
    std::vector<std::string> logLines;

    // Open the log file
    std::ifstream file(GetLogPath().c_str());
    if (!file.is_open()) {
        // Handle error
        return logLines;
    }

    // Read and store each line from the file
    std::string line;
    while (std::getline(file, line)) {
        logLines.push_back(line);
    }

    file.close();

    return logLines;
}
bool Utilities::IsMagicEquipped() { 
    auto player_char = RE::PlayerCharacter::GetSingleton();
    auto equipped_obj_L = player_char->GetEquippedObject(true);
    auto equipped_obj_R = player_char->GetEquippedObject(false);
    bool L_is_magic = equipped_obj_L ? equipped_obj_L->IsMagicItem() : false;
    bool R_is_magic = equipped_obj_R ? equipped_obj_R->IsMagicItem() : false;
    return L_is_magic || R_is_magic;
}
bool Utilities::IsCasting() {
    if (!IsMagicEquipped()) return false;
    auto player_char = RE::PlayerCharacter::GetSingleton();
    auto equipped_obj_L = player_char->GetEquippedObject(true);
    auto equipped_obj_R = player_char->GetEquippedObject(false);
    RE::MagicItem* equipped_obj_L_MI = nullptr;
    RE::MagicItem* equipped_obj_R_MI = nullptr;
    if (equipped_obj_L) equipped_obj_L_MI = equipped_obj_L->As<RE::MagicItem>();
    if (equipped_obj_R) equipped_obj_R_MI = equipped_obj_R->As<RE::MagicItem>();
    bool is_casting = false;
    if (equipped_obj_L_MI && player_char->IsCasting(equipped_obj_L_MI)) is_casting = true;
    if (equipped_obj_R_MI && player_char->IsCasting(equipped_obj_R_MI)) is_casting = true;
    return is_casting;
};