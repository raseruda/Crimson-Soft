#include "CrimsonSoftCore.hpp"

extern "C" FARPROC g_proxy_procs[256] = { nullptr };
extern "C" const char* g_proxy_names[];
extern "C" size_t g_proxy_count;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    return CrimsonSoft::HandleDllMain(hModule, reason, L"\\pdh.dll", g_proxy_procs, g_proxy_names, g_proxy_count);
}
