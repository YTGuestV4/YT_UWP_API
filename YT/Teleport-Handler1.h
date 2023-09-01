#pragma once
#include <iostream>
#include <mutex>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>
#include <fstream>
#include <filesystem>
#include <minwinbase.h>

bool teleport = false;

int aslr_(uintptr_t address)
{
    return address + reinterpret_cast<uintptr_t>(GetModuleHandleA(0));
}

uint64_t check_PlaceId() {
    return Addresses::check_PlaceId;
}

std::string ReadFromFile(const char* filePath) {
    std::ifstream file(filePath);
    if (file.is_open()) {
        std::string content((std::istreambuf_iterator<char>(file)),
            (std::istreambuf_iterator<char>()));
        file.close();
        return content;
    }
    return "";
}

std::string GetUsernameFromEnvironment() {
    char userProfile[MAX_PATH];
    if (GetEnvironmentVariableA("USERPROFILE", userProfile, sizeof(userProfile)) != 0) {
        std::string profilePath(userProfile);
        size_t pos = profilePath.find_last_of("\\");
        if (pos != std::string::npos) {
            return profilePath.substr(pos + 1);
        }
    }
    return "Unknown";
}

int autoexec() {
    std::string username = GetUsernameFromEnvironment();
    std::string Path = "C:\\Users\\" + username + "\\AppData\\Local\\Packages\\ROBLOXCORPORATION.ROBLOX_55nm5eh3cm0pr\\AC\\autoexec\\";

    for (const auto& entry : std::filesystem::directory_iterator(Path)) {
        if (entry.is_regular_file()) {
            std::string filePath = entry.path().string();
            std::string extension = filePath.substr(filePath.find_last_of(".") + 1);
            if (extension == "text" || extension == "txt" || extension == "lua") {
                std::string readContent = ReadFromFile(filePath.c_str());
                Execution::run_script(exthread, (lua_State*)luaL_newstate(), readContent.c_str());
              //Sleep(100);
            }
        }
    }
}

void URS()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    exthread = (uintptr_t)LuaState::get_roblox_state();
    Lua::Bypass(exthread);
    Lua::setlevel(exthread, 8);
    autoexec();
    Addresses::r_lua_print(0, "YT Ready");
}

void detected_teleport_in() {
    if (exthread != LuaState::get_roblox_state() && check_PlaceId() != 0) {
        if (!teleport) {
            teleport = true;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            URS();
            teleport = false;
        }
    }
    else {
        teleport = false;
    }
}

void start_in() {
    while (true) {
        if (!teleport) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            detected_teleport_in();
        }
    }
}
