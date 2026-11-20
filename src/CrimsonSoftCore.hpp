#pragma once
#include "TelemetryCore.hpp"
#include <commctrl.h>
#include <atomic>
#include <mutex>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "comctl32.lib")

namespace CrimsonSoft {

using namespace CDTelemetry;

static HMODULE g_hRealSystemDll = nullptr;
static HMODULE g_hSelfModule = nullptr;
static std::atomic<bool> g_stopThread{ false };
static HANDLE g_hWorkerThread = nullptr;
static HANDLE g_hUiThread = nullptr;
static DWORD g_uiThreadId = 0;

enum ControlIds : int {
    IDC_CHK_MOD_ACTIVE = 1001,
    IDC_CHK_DOF        = 1002,
    IDC_CHK_MBLUR      = 1003,
    IDC_SLD_MBLUR      = 1004,
    IDC_LBL_MBLUR_VAL  = 1005,
    IDC_CHK_BEEP       = 1006,
    IDC_BTN_RESET      = 1007,
    IDC_BTN_CLOSE      = 1008,
    IDC_LBL_STATUS     = 1009
};

struct ModConfig {
    bool modEnabled = true;          // F9: true = _enablePostProcessSharpen = 0 (clean soft image)
    bool enableDoF = true;           // _enablePostProcessDepthOfField (+0x173)
    bool enableMotionBlur = true;    // _enablePostProcessMotionBlur (+0x175)
    float motionBlurScale = 1.0f;    // _masterBlurScale (+0x188) — camera movement blur intensity
    bool beepOnToggle = true;
};

struct OffsetsResolved {
    uint32_t optInRenderer = 0x5E8;
    uint32_t sharpen = 0x172;
    uint32_t dof = 0x173;
    uint32_t motionBlur = 0x175;
    uint32_t masterBlurScale = 0x188;
    uint32_t frameGenMode = 0x2A6;
    uint32_t dlssRecSharp = 0x2A7;
};

static std::mutex g_cfgMutex;
static ModConfig g_cfg;
static OffsetsResolved g_offs;
static std::atomic<uintptr_t> g_globalRendererAddr{ 0 };
static std::atomic<uintptr_t> g_optionPtr{ 0 };
static std::atomic<uint8_t> g_frameGenMode{ 0 };
static std::string g_gameVersion = "?";
static bool g_hasOrig = false;
static uint8_t g_origSharpen = 1;
static uint8_t g_origDof = 1;
static uint8_t g_origMBlur = 1;
static float g_origBlurScale = 1.0f;

static HWND g_hGameWnd = nullptr;
static HWND g_hMenuWnd = nullptr;
static HWND g_hChkMod = nullptr;
static HWND g_hChkDof = nullptr;
static HWND g_hChkMBlur = nullptr;
static HWND g_hSldMBlur = nullptr;
static HWND g_hLblMBlur = nullptr;
static HWND g_hChkBeep = nullptr;
static HWND g_hLblStatus = nullptr;
static HFONT g_hFont = nullptr;
static HFONT g_hFontTitle = nullptr;
static HBRUSH g_hBgBrush = nullptr;

inline bool IsTargetGameProcess() {
    wchar_t path[MAX_PATH]{};
    DWORD len = GetModuleFileNameW(nullptr, path, MAX_PATH);
    if (len == 0) return false;
    const wchar_t* name = wcsrchr(path, L'\\');
    name = name ? (name + 1) : path;
    return _wcsicmp(name, L"CrimsonDesert.exe") == 0;
}

inline BOOL CALLBACK FindGameWindowProc(HWND hwnd, LPARAM lParam) {
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid != GetCurrentProcessId()) return TRUE;
    if (hwnd == g_hMenuWnd) return TRUE;
    if (!IsWindowVisible(hwnd)) return TRUE;
    RECT rc{};
    GetClientRect(hwnd, &rc);
    if ((rc.right - rc.left) >= 640 && (rc.bottom - rc.top) >= 360) {
        *reinterpret_cast<HWND*>(lParam) = hwnd;
        return FALSE;
    }
    return TRUE;
}

inline HWND GetGameMainWindow() {
    if (g_hGameWnd && IsWindow(g_hGameWnd)) return g_hGameWnd;
    HWND found = nullptr;
    EnumWindows(FindGameWindowProc, reinterpret_cast<LPARAM>(&found));
    g_hGameWnd = found;
    return g_hGameWnd;
}

inline bool SafeWriteByte(uintptr_t addr, uint8_t val) {
    __try {
        *reinterpret_cast<volatile uint8_t*>(addr) = val;
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

inline bool SafeWriteFloat(uintptr_t addr, float val) {
    __try {
        *reinterpret_cast<volatile float*>(addr) = val;
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

inline bool SafeReadPtr(uintptr_t addr, uintptr_t& out) {
    __try {
        out = *reinterpret_cast<const volatile uintptr_t*>(addr);
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

inline void ApplyConfigToMemory(uintptr_t optPtr) {
    if (optPtr == 0) return;
    ModConfig c;
    {
        std::lock_guard<std::mutex> lk(g_cfgMutex);
        c = g_cfg;
    }
    SafeWriteByte(optPtr + g_offs.sharpen, c.modEnabled ? 0 : g_origSharpen);
    SafeWriteByte(optPtr + g_offs.dof, c.enableDoF ? 1 : 0);
    SafeWriteByte(optPtr + g_offs.motionBlur, c.enableMotionBlur ? 1 : 0);
    SafeWriteFloat(optPtr + g_offs.masterBlurScale, c.motionBlurScale);
}

inline void UpdateMenuControls() {
    if (!g_hMenuWnd || !IsWindow(g_hMenuWnd)) return;
    ModConfig c;
    {
        std::lock_guard<std::mutex> lk(g_cfgMutex);
        c = g_cfg;
    }
    SendMessageW(g_hChkMod, BM_SETCHECK, c.modEnabled ? BST_CHECKED : BST_UNCHECKED, 0);
    SendMessageW(g_hChkDof, BM_SETCHECK, c.enableDoF ? BST_CHECKED : BST_UNCHECKED, 0);
    SendMessageW(g_hChkMBlur, BM_SETCHECK, c.enableMotionBlur ? BST_CHECKED : BST_UNCHECKED, 0);
    SendMessageW(g_hChkBeep, BM_SETCHECK, c.beepOnToggle ? BST_CHECKED : BST_UNCHECKED, 0);

    int pos = static_cast<int>(std::round(c.motionBlurScale * 100.0f));
    if (pos < 0) pos = 0;
    if (pos > 200) pos = 200;
    SendMessageW(g_hSldMBlur, TBM_SETPOS, TRUE, pos);

    wchar_t mbuf[32]{};
    std::swprintf(mbuf, 32, L"%.2fx", c.motionBlurScale);
    SetWindowTextW(g_hLblMBlur, mbuf);

    uintptr_t p = g_optionPtr.load();
    uint8_t fg = g_frameGenMode.load();
    wchar_t st[160]{};
    if (p != 0) {
        std::swprintf(st, 160, L"Connected v%S | FG: %s | Sharpen: %s",
            g_gameVersion.c_str(),
            fg != 0 ? L"ON" : L"OFF",
            c.modEnabled ? L"OFF (Soft)" : L"ON (Default)");
    } else {
        std::swprintf(st, 160, L"Waiting for game world... | Mod [F9]: %s",
            c.modEnabled ? L"ON" : L"OFF");
    }
    SetWindowTextW(g_hLblStatus, st);
}

inline void PositionMenuInsideGame() {
    if (!g_hMenuWnd || !IsWindow(g_hMenuWnd)) return;
    HWND gameHwnd = GetGameMainWindow();
    if (gameHwnd) {
        POINT pt{ 24, 24 };
        ClientToScreen(gameHwnd, &pt);
        SetWindowPos(g_hMenuWnd, HWND_TOPMOST, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);
    }
}

inline void ToggleMenuVisibility() {
    if (!g_hMenuWnd || !IsWindow(g_hMenuWnd)) return;
    if (IsWindowVisible(g_hMenuWnd)) {
        ShowWindow(g_hMenuWnd, SW_HIDE);
        HWND gameHwnd = GetGameMainWindow();
        if (gameHwnd) SetForegroundWindow(gameHwnd);
    } else {
        UpdateMenuControls();
        PositionMenuInsideGame();
        ShowWindow(g_hMenuWnd, SW_SHOW);
        SetWindowPos(g_hMenuWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        SetForegroundWindow(g_hMenuWnd);
    }
}

inline LRESULT CALLBACK MenuWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            g_hBgBrush = CreateSolidBrush(RGB(26, 28, 34));
            g_hFontTitle = CreateFontW(18, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
            g_hFont = CreateFontW(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

            auto mk = [&](const wchar_t* cls, const wchar_t* txt, DWORD style, int x, int y, int w, int h, int id, HFONT f) {
                HWND hw = CreateWindowExW(0, cls, txt, WS_CHILD | WS_VISIBLE | style,
                    x, y, w, h, hwnd, reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)), g_hSelfModule, nullptr);
                if (f) SendMessageW(hw, WM_SETFONT, reinterpret_cast<WPARAM>(f), TRUE);
                return hw;
            };

            mk(L"STATIC", L"Crimson Soft  [F9 Toggle | F10 Menu]", 0, 14, 10, 330, 22, 0, g_hFontTitle);
            g_hChkMod   = mk(L"BUTTON", L"Disable Post-Process Sharpen [F9] (Soft Image)", BS_AUTOCHECKBOX, 14, 42, 330, 24, IDC_CHK_MOD_ACTIVE, g_hFont);
            g_hChkDof   = mk(L"BUTTON", L"Depth of Field", BS_AUTOCHECKBOX, 14, 70, 330, 24, IDC_CHK_DOF, g_hFont);
            g_hChkMBlur = mk(L"BUTTON", L"Motion Blur", BS_AUTOCHECKBOX, 14, 98, 330, 24, IDC_CHK_MBLUR, g_hFont);

            mk(L"STATIC", L"Motion Blur Intensity (Camera Movement):", 0, 14, 128, 276, 18, 0, g_hFont);
            g_hLblMBlur = mk(L"STATIC", L"1.00x", SS_RIGHT, 294, 128, 50, 18, IDC_LBL_MBLUR_VAL, g_hFont);
            g_hSldMBlur = mk(TRACKBAR_CLASSW, L"", TBS_AUTOTICKS | TBS_HORZ, 10, 148, 338, 28, IDC_SLD_MBLUR, g_hFont);
            SendMessageW(g_hSldMBlur, TBM_SETRANGE, TRUE, MAKELONG(0, 200));
            SendMessageW(g_hSldMBlur, TBM_SETTICFREQ, 25, 0);

            g_hChkBeep  = mk(L"BUTTON", L"Beep on F9 Toggle", BS_AUTOCHECKBOX, 14, 182, 330, 24, IDC_CHK_BEEP, g_hFont);

            mk(L"BUTTON", L"Reset Defaults", BS_PUSHBUTTON, 14, 214, 155, 28, IDC_BTN_RESET, g_hFont);
            mk(L"BUTTON", L"Close [F10]", BS_PUSHBUTTON, 189, 214, 155, 28, IDC_BTN_CLOSE, g_hFont);

            g_hLblStatus = mk(L"STATIC", L"Initializing...", 0, 14, 250, 332, 20, IDC_LBL_STATUS, g_hFont);
            SetTimer(hwnd, 1, 150, nullptr);
            UpdateMenuControls();
            return 0;
        }
        case WM_TIMER: {
            static bool s_hotkeysRegistered = false;
            HWND fgWnd = GetForegroundWindow();
            HWND gameWnd = GetGameMainWindow();
            bool focused = (fgWnd != nullptr) && (fgWnd == gameWnd || fgWnd == hwnd);
            if (focused && !s_hotkeysRegistered) {
                RegisterHotKey(hwnd, 1, MOD_NOREPEAT, VK_F9);
                RegisterHotKey(hwnd, 2, MOD_NOREPEAT, VK_F10);
                s_hotkeysRegistered = true;
            } else if (!focused && s_hotkeysRegistered) {
                UnregisterHotKey(hwnd, 1);
                UnregisterHotKey(hwnd, 2);
                s_hotkeysRegistered = false;
            }
            if (!focused && IsWindowVisible(hwnd)) {
                ShowWindow(hwnd, SW_HIDE);
            }
            return 0;
        }
        case WM_HOTKEY: {
            if (wParam == 1) {
                bool nowOn = false, doBeep = true;
                {
                    std::lock_guard<std::mutex> lk(g_cfgMutex);
                    g_cfg.modEnabled = !g_cfg.modEnabled;
                    nowOn = g_cfg.modEnabled;
                    doBeep = g_cfg.beepOnToggle;
                }
                if (doBeep) Beep(nowOn ? 880 : 440, 65);
                ApplyConfigToMemory(g_optionPtr.load());
                UpdateMenuControls();
            } else if (wParam == 2) {
                ToggleMenuVisibility();
            }
            return 0;
        }
        case WM_HSCROLL: {
            if (reinterpret_cast<HWND>(lParam) == g_hSldMBlur) {
                int pos = static_cast<int>(SendMessageW(g_hSldMBlur, TBM_GETPOS, 0, 0));
                float val = static_cast<float>(pos) / 100.0f;
                { std::lock_guard<std::mutex> lk(g_cfgMutex); g_cfg.motionBlurScale = val; }
                ApplyConfigToMemory(g_optionPtr.load());
                UpdateMenuControls();
            }
            return 0;
        }
        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLORBTN: {
            HDC hdc = reinterpret_cast<HDC>(wParam);
            SetTextColor(hdc, RGB(230, 234, 240));
            SetBkColor(hdc, RGB(26, 28, 34));
            return reinterpret_cast<LRESULT>(g_hBgBrush);
        }
        case WM_ERASEBKGND: {
            HDC hdc = reinterpret_cast<HDC>(wParam);
            RECT rc{};
            GetClientRect(hwnd, &rc);
            FillRect(hdc, &rc, g_hBgBrush);
            return 1;
        }
        case WM_COMMAND: {
            int id = LOWORD(wParam);
            if (id == IDC_CHK_MOD_ACTIVE) {
                bool chk = (SendMessageW(g_hChkMod, BM_GETCHECK, 0, 0) == BST_CHECKED);
                { std::lock_guard<std::mutex> lk(g_cfgMutex); g_cfg.modEnabled = chk; }
                ApplyConfigToMemory(g_optionPtr.load());
                UpdateMenuControls();
            } else if (id == IDC_CHK_DOF) {
                bool chk = (SendMessageW(g_hChkDof, BM_GETCHECK, 0, 0) == BST_CHECKED);
                { std::lock_guard<std::mutex> lk(g_cfgMutex); g_cfg.enableDoF = chk; }
                ApplyConfigToMemory(g_optionPtr.load());
                UpdateMenuControls();
            } else if (id == IDC_CHK_MBLUR) {
                bool chk = (SendMessageW(g_hChkMBlur, BM_GETCHECK, 0, 0) == BST_CHECKED);
                { std::lock_guard<std::mutex> lk(g_cfgMutex); g_cfg.enableMotionBlur = chk; }
                ApplyConfigToMemory(g_optionPtr.load());
                UpdateMenuControls();
            } else if (id == IDC_CHK_BEEP) {
                bool chk = (SendMessageW(g_hChkBeep, BM_GETCHECK, 0, 0) == BST_CHECKED);
                { std::lock_guard<std::mutex> lk(g_cfgMutex); g_cfg.beepOnToggle = chk; }
                UpdateMenuControls();
            } else if (id == IDC_BTN_RESET) {
                {
                    std::lock_guard<std::mutex> lk(g_cfgMutex);
                    g_cfg.modEnabled = true;
                    g_cfg.enableDoF = g_hasOrig ? (g_origDof != 0) : true;
                    g_cfg.enableMotionBlur = g_hasOrig ? (g_origMBlur != 0) : true;
                    g_cfg.motionBlurScale = g_hasOrig ? g_origBlurScale : 1.0f;
                }
                ApplyConfigToMemory(g_optionPtr.load());
                UpdateMenuControls();
            } else if (id == IDC_BTN_CLOSE) {
                ToggleMenuVisibility();
            }
            return 0;
        }
        case WM_CLOSE:
            ShowWindow(hwnd, SW_HIDE);
            return 0;
        case WM_APP + 1:
            ToggleMenuVisibility();
            return 0;
        case WM_APP + 2:
            UpdateMenuControls();
            return 0;
        case WM_APP + 3:
            if (IsWindowVisible(hwnd)) ShowWindow(hwnd, SW_HIDE);
            return 0;
        case WM_DESTROY:
            KillTimer(hwnd, 1);
            UnregisterHotKey(hwnd, 1);
            UnregisterHotKey(hwnd, 2);
            if (g_hFont) { DeleteObject(g_hFont); g_hFont = nullptr; }
            if (g_hFontTitle) { DeleteObject(g_hFontTitle); g_hFontTitle = nullptr; }
            if (g_hBgBrush) { DeleteObject(g_hBgBrush); g_hBgBrush = nullptr; }
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

inline DWORD WINAPI UiThreadProc(LPVOID) {
    for (int i = 0; i < 150 && !g_stopThread.load(); ++i) {
        if (GetGameMainWindow() != nullptr) break;
        Sleep(100);
    }
    if (g_stopThread.load()) return 0;

    INITCOMMONCONTROLSEX icex{ sizeof(icex), ICC_BAR_CLASSES | ICC_STANDARD_CLASSES };
    InitCommonControlsEx(&icex);

    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = MenuWndProc;
    wc.hInstance = g_hSelfModule;
    wc.hCursor = LoadCursorW(nullptr, reinterpret_cast<LPCWSTR>(IDC_ARROW));
    wc.hbrBackground = nullptr;
    wc.lpszClassName = L"CrimsonSoftOverlayWnd";
    RegisterClassExW(&wc);

    g_hMenuWnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        wc.lpszClassName,
        L"Crimson Soft",
        WS_POPUP | WS_BORDER,
        24, 24, 360, 278,
        GetGameMainWindow(), nullptr, g_hSelfModule, nullptr);

    MSG msg{};
    while (!g_stopThread.load() && GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}

inline uintptr_t FindGlobalSceneRendererPtrAddr(const uint8_t* code, uint32_t codeRva, uint32_t codeSize, uintptr_t imageBase, uint32_t optOff, uint32_t dlssRecOff) {
    for (uint32_t i = 0; i + 24 < codeSize; ++i) {
        if (code[i] == 0x48 && code[i + 1] == 0x8B && code[i + 2] == 0x05 &&
            code[i + 7] == 0x0F && code[i + 8] == 0x9F && code[i + 9] == 0xC2 &&
            code[i + 10] == 0x48 && code[i + 11] == 0x8B && code[i + 12] == 0x88 &&
            code[i + 17] == 0x88 && code[i + 18] == 0x91 && code[i + 23] == 0xC3) {
            uint32_t foundOptOff = 0, foundRecOff = 0;
            std::memcpy(&foundOptOff, code + i + 13, 4);
            std::memcpy(&foundRecOff, code + i + 19, 4);
            if (foundOptOff == optOff && foundRecOff == dlssRecOff) {
                int32_t rel = 0;
                std::memcpy(&rel, code + i + 3, 4);
                uintptr_t nextRip = imageBase + codeRva + i + 7;
                return static_cast<uintptr_t>(static_cast<intptr_t>(nextRip) + rel);
            }
        }
    }
    return 0;
}

inline bool ValidateBinaryInMemory(const InProcessReader& reader, BinaryValidationResult& outRes) {
    wchar_t exePathW[MAX_PATH]{};
    GetModuleFileNameW(nullptr, exePathW, MAX_PATH);
    char utf8Path[MAX_PATH * 2]{};
    WideCharToMultiByte(CP_UTF8, 0, exePathW, -1, utf8Path, sizeof(utf8Path), nullptr, nullptr);
    outRes.exePath = utf8Path;
    outRes.fileVersion = QueryExeVersionString(exePathW, outRes.productVersion, outRes.fileSize);

    uintptr_t base = reader.GetImageBase();
    if (base == 0) return false;

    uint8_t hdr[4096]{};
    if (!reader.ReadBytes(base, hdr, sizeof(hdr))) return false;

    auto* dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(hdr);
    auto* nt = reinterpret_cast<const IMAGE_NT_HEADERS64*>(hdr + dos->e_lfanew);
    if (dos->e_magic != IMAGE_DOS_SIGNATURE || nt->Signature != IMAGE_NT_SIGNATURE) return false;

    outRes.peTimeDateStamp = nt->FileHeader.TimeDateStamp;
    outRes.sizeOfImage = nt->OptionalHeader.SizeOfImage;

    auto* sec = IMAGE_FIRST_SECTION(nt);
    for (WORD s = 0; s < nt->FileHeader.NumberOfSections; ++s) {
        if (s == 0) {
            outRes.codeRva = sec[s].VirtualAddress;
            outRes.codeSize = sec[s].SizeOfRawData;
        } else if (s == 1) {
            outRes.rdataRva = sec[s].VirtualAddress;
            outRes.rdataSize = sec[s].SizeOfRawData;
        }
    }
    if (outRes.codeSize == 0 || outRes.rdataSize == 0) return false;
    if (!InProcessReader::IsPageReadable(base + outRes.codeRva, outRes.codeSize) ||
        !InProcessReader::IsPageReadable(base + outRes.rdataRva, outRes.rdataSize)) {
        return false;
    }

    const uint8_t* codePtr = reinterpret_cast<const uint8_t*>(base + outRes.codeRva);
    const uint8_t* rdataPtr = reinterpret_cast<const uint8_t*>(base + outRes.rdataRva);

    RunDynamicScannerOnBuffers(
        codePtr, outRes.codeRva, outRes.codeSize,
        rdataPtr, outRes.rdataRva, outRes.rdataSize,
        outRes);

    auto resolve = [&](const char* name, uint32_t fb) -> uint32_t {
        auto it = outRes.resolvedOffsets.find(name);
        return (it != outRes.resolvedOffsets.end()) ? it->second : fb;
    };
    g_offs.optInRenderer = outRes.optionPtrOffsetInRenderer;
    g_offs.sharpen = resolve("_enablePostProcessSharpen", 0x172);
    g_offs.dof = resolve("_enablePostProcessDepthOfField", 0x173);
    g_offs.motionBlur = resolve("_enablePostProcessMotionBlur", 0x175);
    g_offs.masterBlurScale = resolve("_masterBlurScale", 0x188);
    g_offs.frameGenMode = resolve("_frameGenerationMode", 0x2A6);
    g_offs.dlssRecSharp = resolve("_useRecommendedSharpnessForDlss", 0x2A7);

    uintptr_t globAddr = FindGlobalSceneRendererPtrAddr(
        codePtr, outRes.codeRva, outRes.codeSize, base, g_offs.optInRenderer, g_offs.dlssRecSharp);
    g_globalRendererAddr.store(globAddr);
    return true;
}

inline uintptr_t ResolveLiveOptionPtr(const InProcessReader& reader, const BinaryValidationResult& binVal) {
    uintptr_t globAddr = g_globalRendererAddr.load();
    if (globAddr != 0) {
        uintptr_t renderer = 0;
        if (SafeReadPtr(globAddr, renderer) && renderer > 0x10000 && renderer < 0x00007FFFFFFFFFFFULL) {
            uintptr_t opt = 0;
            if (SafeReadPtr(renderer + g_offs.optInRenderer, opt) && opt > 0x10000 && opt < 0x00007FFFFFFFFFFFULL) {
                uintptr_t back0 = 0, back8 = 0;
                if (SafeReadPtr(opt + 0x00, back0) && SafeReadPtr(opt + 0x08, back8) &&
                    back0 == renderer && back8 == renderer) {
                    return opt;
                }
            }
        }
    }
    RuntimeInstanceState rt = LocateAndReadSceneRenderer(reader, binVal, nullptr);
    if (rt.sceneRendererFound && rt.optionStructFound && rt.bidirectionalLockVerified) {
        return rt.optionStructPtr;
    }
    return 0;
}

inline DWORD WINAPI ModWorkerThread(LPVOID) {
    for (int i = 0; i < 40 && !g_stopThread.load(); ++i) Sleep(100);
    if (g_stopThread.load()) return 0;

    InProcessReader reader;
    BinaryValidationResult binVal{};
    if (!ValidateBinaryInMemory(reader, binVal)) return 0;
    g_gameVersion = binVal.fileVersion;

    while (!g_stopThread.load()) {
        uintptr_t prevOpt = g_optionPtr.load();
        uintptr_t liveOpt = (g_globalRendererAddr.load() != 0 || prevOpt == 0)
            ? ResolveLiveOptionPtr(reader, binVal)
            : prevOpt;

        if (liveOpt != 0) {
            if (!g_hasOrig) {
                reader.Read(liveOpt + g_offs.sharpen, g_origSharpen);
                reader.Read(liveOpt + g_offs.dof, g_origDof);
                reader.Read(liveOpt + g_offs.motionBlur, g_origMBlur);
                reader.Read(liveOpt + g_offs.masterBlurScale, g_origBlurScale);
                {
                    std::lock_guard<std::mutex> lk(g_cfgMutex);
                    g_cfg.enableDoF = (g_origDof != 0);
                    g_cfg.enableMotionBlur = (g_origMBlur != 0);
                    g_cfg.motionBlurScale = g_origBlurScale;
                }
                g_hasOrig = true;
            }
            uint8_t fg = 0;
            reader.Read(liveOpt + g_offs.frameGenMode, fg);
            uint8_t prevFg = g_frameGenMode.exchange(fg);

            g_optionPtr.store(liveOpt);
            ApplyConfigToMemory(liveOpt);

            if (liveOpt != prevOpt || fg != prevFg) {
                if (g_hMenuWnd) PostMessageW(g_hMenuWnd, WM_APP + 2, 0, 0);
            }
        }

        Sleep(250);
    }

    uintptr_t optPtr = g_optionPtr.load();
    if (optPtr != 0 && g_hasOrig) {
        SafeWriteByte(optPtr + g_offs.sharpen, g_origSharpen);
        SafeWriteByte(optPtr + g_offs.dof, g_origDof);
        SafeWriteByte(optPtr + g_offs.motionBlur, g_origMBlur);
        SafeWriteFloat(optPtr + g_offs.masterBlurScale, g_origBlurScale);
    }
    return 0;
}

inline void InitSystemDllProxy(const wchar_t* sysSubPath, FARPROC* procs, const char* const* names, size_t count) {
    if (g_hRealSystemDll) return;
    wchar_t sysDir[MAX_PATH]{};
    GetSystemDirectoryW(sysDir, MAX_PATH);
    std::wstring realPath = std::wstring(sysDir) + sysSubPath;
    g_hRealSystemDll = LoadLibraryW(realPath.c_str());
    if (!g_hRealSystemDll) return;

    for (size_t i = 0; i < count; ++i) {
        procs[i] = GetProcAddress(g_hRealSystemDll, names[i]);
    }
}

inline BOOL HandleDllMain(
    HMODULE hModule,
    DWORD reason,
    const wchar_t* sysSubPath,
    FARPROC* procs,
    const char* const* names,
    size_t count)
{
    if (reason == DLL_PROCESS_ATTACH) {
        g_hSelfModule = hModule;
        DisableThreadLibraryCalls(hModule);
        InitSystemDllProxy(sysSubPath, procs, names, count);
        if (IsTargetGameProcess()) {
            g_stopThread.store(false);
            g_hUiThread = CreateThread(nullptr, 0, UiThreadProc, nullptr, 0, &g_uiThreadId);
            g_hWorkerThread = CreateThread(nullptr, 0, ModWorkerThread, nullptr, 0, nullptr);
        }
    } else if (reason == DLL_PROCESS_DETACH) {
        g_stopThread.store(true);
        if (g_hMenuWnd) PostMessageW(g_hMenuWnd, WM_CLOSE, 0, 0);
        if (g_uiThreadId) PostThreadMessageW(g_uiThreadId, WM_QUIT, 0, 0);
        if (g_hWorkerThread) {
            WaitForSingleObject(g_hWorkerThread, 500);
            CloseHandle(g_hWorkerThread);
            g_hWorkerThread = nullptr;
        }
        if (g_hUiThread) {
            WaitForSingleObject(g_hUiThread, 500);
            CloseHandle(g_hUiThread);
            g_hUiThread = nullptr;
        }
    }
    return TRUE;
}

} // namespace CrimsonSoft
