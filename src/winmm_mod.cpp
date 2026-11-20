#include "CrimsonSoftCore.hpp"

extern "C" FARPROC g_winmm_procs[256] = { nullptr };
extern "C" const char* g_winmm_names[256];
extern "C" size_t g_winmm_count;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    return CrimsonSoft::HandleDllMain(hModule, reason, L"\\winmm.dll", g_winmm_procs, g_winmm_names, g_winmm_count);
}
