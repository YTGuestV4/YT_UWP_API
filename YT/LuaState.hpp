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
    int get_job_by_name(std::string job_name)
    {
        auto job_return = 0;

        auto job_start = *(uintptr_t*)(Addresses::r_getscheduler() + 308);

        while (job_start != *(uintptr_t*)(Addresses::r_getscheduler() + 312)) // *(this + 312) = 0;
        {
            if (*(std::string*)(*(uintptr_t*)(job_start) + 128) == job_name) // updated
            {
                job_return = *(uintptr_t*)(job_start);
            }
            job_start += 8;
        }       
        if (Debug == true) {
            std::string job_returnv = "job_return value: " + std::to_string(job_return);
            Addresses::r_lua_print(0, const_cast<char*>(job_returnv.c_str()));
        }
        return job_return;
    }

    int get_script_context()
    {
        auto script_context = *(uintptr_t*)(get_job_by_name("WaitingHybridScriptsJob") + 424); // old 304 *(this + 312) = a2;
        if (Debug == true) {
            std::string get_script_contextvalue = "script_context value: " + std::to_string(script_context);
            Addresses::r_lua_print(0, const_cast<char*>(get_script_contextvalue.c_str()));
        }
        return script_context;
    }

     int get_robloxstate()
     {
         return Deobfuscation::luastate(get_script_context());
     }

    //int get_robloxstate()
    //{
    //    int identity = 0;
    //    int script = 0;
    //    uintptr_t script_context_address = get_script_context();
    //    uintptr_t roblox_state_address = Addresses::get_global_state_2(script_context_address, &identity, &script);
    //    return roblox_state_address;
    //}
}
