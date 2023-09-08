#include <iostream>
#include <Windows.h>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include "Addresses.hpp"

namespace LuaState
{
    int get_job_by_name(std::string job_name)
    {
        auto job_return = 0;

        auto job_start = *(uintptr_t*)(Addresses::r_getscheduler() + 308);

        while (job_start != *(uintptr_t*)(Addresses::r_getscheduler() + 312)) // *(this + 316) = 0;
        {
            if (*(std::string*)(*(uintptr_t*)(job_start)+128) == job_name)  // 0x80 128
            {
                job_return = *(uintptr_t*)(job_start);
            }
            job_start += 8;
        }

        return job_return;
    }

    int get_script_context()
    {
        auto script_context = *(uintptr_t*)(get_job_by_name("WaitingHybridScriptsJob") + 424); // 424  0x1A8
        return script_context;
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
