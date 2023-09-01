#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include "Addresses.hpp"

bool Debug = false;
uintptr_t lua_state_global;


namespace LuaState
{
    int get_job_by_name(const std::string& job_name) {
        auto job_return = 0;

        auto job_start = *reinterpret_cast<uintptr_t*>(Addresses::r_getscheduler() + 0x134);
        auto job_end = *reinterpret_cast<uintptr_t*>(Addresses::r_getscheduler() + 0x138);

        while (job_start != job_end) {
            auto job_name_ptr = *reinterpret_cast<std::string*>(*reinterpret_cast<uintptr_t*>(job_start) + 0x80);

            if (job_name_ptr == job_name) {
                job_return = *reinterpret_cast<uintptr_t*>(job_start);
                break;
            }

            job_start += 8;
        }

        return job_return;
    }

    int get_script_context() {

        const std::string job_name = "WaitingHybridScriptsJob";

        uintptr_t job = get_job_by_name(job_name);

        if (job == 0) {
            Addresses::r_lua_print(0, "oh god not again");
            return 0;
        }

        auto script_context = *reinterpret_cast<uintptr_t*>(job + 0x1A8);
        return static_cast<int>(script_context);
    }


    int get_roblox_state()
    {
        int identity = 8;
        int script = 0;

        uintptr_t script_context_address = get_script_context();
        uintptr_t roblox_state_address = Addresses::get_global_state_2(script_context_address, &identity, &script);
        return roblox_state_address;
    }
}
