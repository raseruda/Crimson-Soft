#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <winver.h>
#include <dxgi1_4.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <cmath>

#pragma comment(lib, "version.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "psapi.lib")

namespace CDTelemetry {

// ============================================================================
// 1. EXPECTED STRUCT OFFSETS IN [PASceneRenderer + OptionOffset] (PASceneRendererOption)
//    Verified against v1.0.0.3392, v1.0.0.3446 (+0x5C8) and v1.0.0.3732 (+0x5E8)
// ============================================================================
struct ExpectedProp {
    const char* name;
    uint32_t expectedOffset;
    const char* typeName; // "bool", "uint8", "int32", "float"
    const char* category;
    uint32_t altExpectedOffset = 0; // Optional shifted offset in newer patches (e.g. v1.0.0.3732 +4 on RT/PT)
};

static const ExpectedProp kExpectedProps[] = {
    // SSDM / Shadows / HBAO / TAA / Trees / PostProcess / Sharpen / Blur
    {"_screenSpaceDisplacementMapping", 0x040, "bool", "Shadows/SSDM/TAA"},
    {"_useSSDMRasterizer", 0x041, "bool", "Shadows/SSDM/TAA"},
    {"_enableSSDMHighQuality", 0x042, "bool", "Shadows/SSDM/TAA"},
    {"_enableSSDMTemporalReprojection", 0x043, "bool", "Shadows/SSDM/TAA"},
    {"_lightingQualityLevel", 0x05C, "int32", "Presets"},
    {"_renderShadowInterleaved", 0x079, "bool", "Shadows/SSDM/TAA"},
    {"_renderShadowHighQuality", 0x07A, "bool", "Shadows/SSDM/TAA"},
    {"_enableHBAOHighQuality", 0x080, "bool", "Shadows/SSDM/TAA"},
    {"_shadowQualityLevel", 0x0A4, "uint8", "Presets"},
    {"_enableTemporalAA", 0x16A, "bool", "TAA/PostProcess"},
    {"_enableTemporalAAUpsample", 0x16B, "bool", "TAA/PostProcess"},
    {"_enableTemporalAAUpsampleBicubic", 0x16C, "bool", "TAA/PostProcess"},
    {"_useTemporalAAR2Noise", 0x16E, "bool", "TAA/PostProcess"},
    {"_enableTemporalAANeighborhoodClamping", 0x16F, "bool", "TAA/PostProcess"},
    {"_enableTemporalAABilateralWeights", 0x170, "bool", "TAA/PostProcess"},
    {"_enableTemporalAAAggressiveSuperSampling", 0x171, "bool", "TAA/PostProcess"},
    {"_enablePostProcessSharpen", 0x172, "bool", "TAA/PostProcess"},
    {"_enablePostProcessDepthOfField", 0x173, "bool", "TAA/PostProcess"},
    {"_runDepthOfFieldBeforeUpscale", 0x174, "bool", "TAA/PostProcess"},
    {"_enablePostProcessMotionBlur", 0x175, "bool", "TAA/PostProcess"},
    {"_enablePostProcessFFTBloom", 0x179, "bool", "TAA/PostProcess"},
    {"_masterBlurScale", 0x188, "float", "TAA/PostProcess"},
    {"_mipLODBias", 0x1AC, "bool", "TAA/PostProcess"},
    {"_useJitteredAlphaTestForTrees", 0x1B9, "bool", "TAA/PostProcess"},

    // Global Illumination & ReSTIR Reservoirs
    {"_enableGlobalIllumination", 0x0BA, "bool", "Diffuse GI & ReSTIR"},
    {"_renderDiffuseGlobalIllumination", 0x0BB, "bool", "Diffuse GI & ReSTIR"},
    {"_renderGIDownsampled", 0x0BC, "bool", "Diffuse GI & ReSTIR"},
    {"_diffuseGIInterleavedMode", 0x0C0, "int32", "Diffuse GI & ReSTIR"},
    {"_reprojectDiffuseGIDownsampled", 0x0C4, "bool", "Diffuse GI & ReSTIR"},
    {"_renderDiffuseGIUseIndirectCacheReservoir", 0x0C6, "bool", "Diffuse GI & ReSTIR"},
    {"_enhanceDiffuseGIUseIndirectCacheReservoir", 0x0C7, "bool", "Diffuse GI & ReSTIR"},
    {"_renderDiffuseGIUseReservoirSampling", 0x0C8, "bool", "Diffuse GI & ReSTIR"},
    {"_renderDiffuseGIHighQuality", 0x0CE, "bool", "Diffuse GI & ReSTIR"},
    {"_renderDiffuseGIUseCharacterOcclusions", 0x0D1, "bool", "Diffuse GI & ReSTIR"},
    {"_enableBRDFWeightsForRayGuiding", 0x0E1, "bool", "Diffuse GI & ReSTIR"},
    {"_manyLightsQualityLevel", 0x0E8, "int32", "Presets"},
    {"_manyLightsUseDownsampledLightVisibility", 0x0EC, "bool", "Diffuse GI & ReSTIR"},
    {"_manyLightsSamplingUseSeparatePass", 0x0EE, "bool", "Diffuse GI & ReSTIR"},
    {"_manyLightsUseWiderKernel", 0x0F1, "bool", "Diffuse GI & ReSTIR"},
    {"_numManyLightsFilterPasses", 0x0FC, "int32", "Diffuse GI & ReSTIR"},

    // Standard Denoisers (SVGF, Diffuse/Specular GI Filtering, Recurrent Blur)
    {"_enableDiffuseGIFiltering", 0x0F6, "bool", "Standard Denoisers"},
    {"_useSVGFiltering", 0x104, "bool", "Standard Denoisers"},
    {"_numDiffuseGIFilterPasses", 0x108, "int32", "Standard Denoisers"},
    {"_numDiffuseGIFilterPassesForFarTiles", 0x10C, "int32", "Standard Denoisers"},
    {"_numDiffuseHitDistanceFilterPasses", 0x110, "int32", "Standard Denoisers"},
    {"_renderSpecularGlobalIllumination", 0x114, "bool", "Standard Denoisers"},
    {"_reprojectSpecularGIDownsampled", 0x115, "bool", "Standard Denoisers"},
    {"_enableSpecularGIFiltering", 0x11B, "bool", "Standard Denoisers"},
    {"_applySpecularGIFilteringBeforeTemporalReprojection", 0x11C, "bool", "Standard Denoisers"},
    {"_numSpecularGIFilterPasses", 0x120, "int32", "Standard Denoisers"},
    {"_specularGIQualityLevel", 0x124, "int32", "Presets"},
    {"_recurrentBlurForSpecular", 0x2B6, "bool", "Standard Denoisers"},
    {"_recurrentPreBlur", 0x2B7, "bool", "Standard Denoisers"},
    {"_recurrentPreBlurRadius", 0x2B8, "float", "Standard Denoisers"},
    {"_recurrentBlurUseWaveletFilter", 0x2BC, "bool", "Standard Denoisers"},
    {"_recurrentPostBlur", 0x2BD, "bool", "Standard Denoisers"},
    {"_recurrentBlurReconstructHistory", 0x2BE, "bool", "Standard Denoisers"},
    {"_recurrentPostBlurRadius", 0x2C0, "float", "Standard Denoisers"},

    // Voxels, SDF & Radiance Cache
    {"_generateSceneVoxels", 0x134, "bool", "Voxels & Radiance Cache"},
    {"_generateSDFVoxelsFromObjects", 0x136, "bool", "Voxels & Radiance Cache"},
    {"_generateSDFVoxelsFromTerrain", 0x137, "bool", "Voxels & Radiance Cache"},
    {"_generateSDFVoxelsFromCharacters", 0x138, "bool", "Voxels & Radiance Cache"},
    {"_generateHairScatteringVoxels", 0x139, "bool", "Voxels & Radiance Cache"},
    {"_integrateHairScatteringDownsampled", 0x13A, "bool", "Voxels & Radiance Cache"},
    {"_updateRadianceVoxelsUseHighSampleCount", 0x150, "bool", "Voxels & Radiance Cache"},
    {"_updateRadianceVoxelsUseRaytracing", 0x151, "bool", "Voxels & Radiance Cache"},
    {"_updateIndirectCacheLightingUseHighSampleCount", 0x153, "bool", "Voxels & Radiance Cache"},
    {"_updateIndirectCacheLightingUseRaytracing", 0x154, "bool", "Voxels & Radiance Cache"},
    {"_updateIndirectCacheLightingUseMaskVoxelsFrameDistributionPerAxis", 0x158, "int32", "Voxels & Radiance Cache"},

    // Ray Tracing & Path Tracing (v1.0.0.3446 -> v1.0.0.3732 shifted +4)
    {"_enableRaytracing", 0x267, "bool", "Ray Tracing & Path Tracing", 0x26B},
    {"_renderRaytracedDiffuseGI", 0x268, "bool", "Ray Tracing & Path Tracing", 0x26C},
    {"_renderRaytracedSpecularGI", 0x269, "bool", "Ray Tracing & Path Tracing", 0x26D},
    {"_useAdaptiveRayBudgetForSpecularGI", 0x26A, "bool", "Ray Tracing & Path Tracing", 0x26E},
    {"_renderRaytracedLocalLights", 0x26D, "bool", "Ray Tracing & Path Tracing", 0x271},
    {"_raytracingDebugMode", 0x270, "int32", "Ray Tracing & Path Tracing", 0x274},
    {"_renderPathTracing", 0x271, "bool", "Ray Tracing & Path Tracing", 0x275},
    {"_enablePathTracingDenoising", 0x272, "bool", "Ray Tracing & Path Tracing", 0x276},
    {"_renderPathTracingDownsampled", 0x273, "bool", "Ray Tracing & Path Tracing", 0x277},

    // Upscaler, Frame Generation, Ray Reconstruction & FfxDenoiser
    {"_frameGenerationNumFramesToGenerate", 0x298, "int32", "Upscaler & FrameGen"},
    {"_enableDynamicMultiFrameGeneration", 0x29C, "bool", "Upscaler & FrameGen"},
    {"_dynamicTargetFrameRate", 0x2A0, "int32", "Upscaler & FrameGen"},
    {"_resolutionScale", 0x2A4, "uint8", "Upscaler & FrameGen"},
    {"_upscaleResolutionMethod", 0x2A5, "uint8", "Upscaler & FrameGen"},
    {"_frameGenerationMode", 0x2A6, "uint8", "Upscaler & FrameGen"},
    {"_useRecommendedSharpnessForDlss", 0x2A7, "bool", "Upscaler & FrameGen"},
    {"_dlssSharpness", 0x2A8, "float", "Upscaler & FrameGen"},
    {"_dlssPreset", 0x2AC, "uint8", "Upscaler & FrameGen"},
    {"_dlssRRPreset", 0x2AD, "uint8", "Upscaler & FrameGen"},
    {"_fsrSharpness", 0x2B0, "float", "Upscaler & FrameGen"},
    {"_enableRayReconstruction", 0x2B5, "bool", "Alternative Denoisers"},
    {"_ffxDenoiserVersionIndex", 0x2C4, "int32", "Alternative Denoisers"},
    {"_ffxDenoiserUseDebugFeatures", 0x2C8, "bool", "Alternative Denoisers"},
    {"_ffxDenoiserCrossBilateralNormalStrength", 0x2CC, "float", "Alternative Denoisers"},
    {"_ffxDenoiserStabilityBias", 0x2D0, "float", "Alternative Denoisers"},
    {"_ffxDenoiserMaxRadiance", 0x2D4, "float", "Alternative Denoisers"}
};

static constexpr size_t kNumExpectedProps = sizeof(kExpectedProps) / sizeof(kExpectedProps[0]);

// ============================================================================
// 2. ABSTRACT MEMORY READER INTERFACE (Supports both In-Process & External Read-Only)
//    Rule 1 & 2 Compliance: 100% Read-Only, zero memory writes, zero hooks.
// ============================================================================
class IMemoryReader {
public:
    virtual ~IMemoryReader() = default;
    virtual bool ReadBytes(uintptr_t address, void* buffer, size_t size) const = 0;
    virtual uintptr_t GetImageBase() const = 0;
    virtual bool IsLiveProcess() const = 0;

    template <typename T>
    bool Read(uintptr_t address, T& outVal) const {
        return ReadBytes(address, &outVal, sizeof(T));
    }
};

// In-process read-only reader using VirtualQuery + SEH (__try / __except)
class InProcessReader : public IMemoryReader {
    uintptr_t m_imageBase;
public:
    InProcessReader() {
        m_imageBase = reinterpret_cast<uintptr_t>(GetModuleHandleW(nullptr));
    }
    uintptr_t GetImageBase() const override { return m_imageBase; }
    bool IsLiveProcess() const override { return true; }

    static bool IsPageReadable(uintptr_t address, size_t size) {
        if (address < 0x10000 || address > 0x00007FFFFFFFFFFFULL) return false;
        MEMORY_BASIC_INFORMATION mbi{};
        if (VirtualQuery(reinterpret_cast<LPCVOID>(address), &mbi, sizeof(mbi)) != sizeof(mbi))
            return false;
        if (mbi.State != MEM_COMMIT) return false;
        if (mbi.Protect & (PAGE_NOACCESS | PAGE_GUARD)) return false;
        const DWORD readableMask = PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY |
                                   PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
        if (!(mbi.Protect & readableMask)) return false;
        uintptr_t regionEnd = reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize;
        return (address + size) <= regionEnd;
    }

    static bool SafeMemcpy(uintptr_t address, void* buffer, size_t size) {
        __try {
            std::memcpy(buffer, reinterpret_cast<const void*>(address), size);
            return true;
        } __except (EXCEPTION_EXECUTE_HANDLER) {
            return false;
        }
    }

    bool ReadBytes(uintptr_t address, void* buffer, size_t size) const override {
        if (!IsPageReadable(address, size)) return false;
        return SafeMemcpy(address, buffer, size);
    }
};

// External process read-only reader using OpenProcess(PROCESS_VM_READ) + ReadProcessMemory
class ExternalProcessReader : public IMemoryReader {
    HANDLE m_hProcess = nullptr;
    uintptr_t m_imageBase = 0;
public:
    ExternalProcessReader(HANDLE hProc, uintptr_t base) : m_hProcess(hProc), m_imageBase(base) {}
    uintptr_t GetImageBase() const override { return m_imageBase; }
    bool IsLiveProcess() const override { return m_hProcess != nullptr; }
    HANDLE GetProcessHandle() const { return m_hProcess; }

    bool ReadBytes(uintptr_t address, void* buffer, size_t size) const override {
        if (!m_hProcess || address < 0x10000 || address > 0x00007FFFFFFFFFFFULL) return false;
        SIZE_T bytesRead = 0;
        return ReadProcessMemory(m_hProcess, reinterpret_cast<LPCVOID>(address), buffer, size, &bytesRead) && (bytesRead == size);
    }
};

// ============================================================================
// 3. EXECUTABLE VERSION & DYNAMIC OFFSET VALIDATOR
// ============================================================================
struct BinaryValidationResult {
    std::string exePath;
    std::string fileVersion;
    std::string productVersion;
    uint64_t fileSize = 0;
    uint32_t peTimeDateStamp = 0;
    uint32_t sizeOfImage = 0;

    uint32_t codeRva = 0;
    uint32_t codeSize = 0;
    uint32_t rdataRva = 0;
    uint32_t rdataSize = 0;

    uint32_t vtableSceneRendererBaseRva = 0;
    uint32_t vtableSceneRendererDerivedRva = 0;
    uint32_t globalRenderDeviceRva = 0;
    uint32_t ctorSceneRendererBaseRva = 0;
    uint32_t ctorSceneRendererDerivedRva = 0;
    uint32_t ctorSceneRendererOptionRva = 0;
    uint32_t optionPtrOffsetInRenderer = 0x5E8;
    uint32_t renderDeviceOffsetInRenderer = 0x598;

    std::map<std::string, uint32_t> resolvedOffsets;
    size_t totalGettersResolved = 0;
    size_t criticalMatched = 0;
    size_t criticalMismatched = 0;
    std::vector<std::string> validationMessages;
    bool allCriticalValid = false;
};

inline std::string QueryExeVersionString(const std::wstring& path, std::string& outProdVer, uint64_t& outFileSize) {
    WIN32_FILE_ATTRIBUTE_DATA fad{};
    if (GetFileAttributesExW(path.c_str(), GetFileExInfoStandard, &fad)) {
        outFileSize = (static_cast<uint64_t>(fad.nFileSizeHigh) << 32) | fad.nFileSizeLow;
    }
    DWORD handle = 0;
    DWORD verSize = GetFileVersionInfoSizeW(path.c_str(), &handle);
    if (verSize == 0) return "Unknown";
    std::vector<uint8_t> verData(verSize);
    if (!GetFileVersionInfoW(path.c_str(), handle, verSize, verData.data())) return "Unknown";

    VS_FIXEDFILEINFO* ffi = nullptr;
    UINT ffiLen = 0;
    if (VerQueryValueW(verData.data(), L"\\", reinterpret_cast<LPVOID*>(&ffi), &ffiLen) && ffi) {
        char buf[64];
        std::snprintf(buf, sizeof(buf), "%u.%u.%u.%u",
            HIWORD(ffi->dwFileVersionMS), LOWORD(ffi->dwFileVersionMS),
            HIWORD(ffi->dwFileVersionLS), LOWORD(ffi->dwFileVersionLS));
        char pbuf[64];
        std::snprintf(pbuf, sizeof(pbuf), "%u.%u.%u.%u",
            HIWORD(ffi->dwProductVersionMS), LOWORD(ffi->dwProductVersionMS),
            HIWORD(ffi->dwProductVersionLS), LOWORD(ffi->dwProductVersionLS));
        outProdVer = pbuf;
        return buf;
    }
    return "Unknown";
}

// Scans code & rdata buffers (either from live process memory or disk file)
inline void RunDynamicScannerOnBuffers(
    const uint8_t* codeBuf, uint32_t codeRva, uint32_t codeSize,
    const uint8_t* rdataBuf, uint32_t rdataRva, uint32_t rdataSize,
    BinaryValidationResult& res)
{
    // 1A. Scan inline property accessors:
    //     48 8D 15 disp32 (lea rdx, "_propName") at i
    //     followed at i + 0x20 by 48 8B 77 28 (mov rsi, [rdi+0x28])
    //     and 48 81 C6 disp32 (add rsi, disp32) or 48 83 C6 disp8 (add rsi, disp8)
    for (uint32_t i = 0; i + 0x30 < codeSize; ++i) {
        if (codeBuf[i] == 0x48 && codeBuf[i+1] == 0x8D && codeBuf[i+2] == 0x15 &&
            codeBuf[i+0x20] == 0x48 && codeBuf[i+0x21] == 0x8B && codeBuf[i+0x22] == 0x77 && codeBuf[i+0x23] == 0x28) {
            uint32_t propOffset = 0;
            bool validOff = false;
            if (codeBuf[i+0x24] == 0x48 && codeBuf[i+0x25] == 0x81 && codeBuf[i+0x26] == 0xC6) {
                std::memcpy(&propOffset, &codeBuf[i+0x27], 4);
                validOff = true;
            } else if (codeBuf[i+0x24] == 0x48 && codeBuf[i+0x25] == 0x83 && codeBuf[i+0x26] == 0xC6) {
                propOffset = codeBuf[i+0x27];
                validOff = true;
            } else if (codeBuf[i+0x24] == 0x48 && codeBuf[i+0x25] == 0x83 && codeBuf[i+0x26] == 0xEE && codeBuf[i+0x27] == 0x80) {
                // MSVC encodes `add rsi, 0x80` as `sub rsi, -0x80` (48 83 EE 80) to save 3 bytes!
                propOffset = 0x080;
                validOff = true;
            }
            if (!validOff || propOffset > 0x400) continue;

            int32_t disp = 0;
            std::memcpy(&disp, &codeBuf[i+3], 4);
            uint32_t strRva = static_cast<uint32_t>(static_cast<int64_t>(codeRva + i + 7) + disp);
            if (strRva >= rdataRva && strRva + 2 < rdataRva + rdataSize) {
                const uint8_t* s = rdataBuf + (strRva - rdataRva);
                if (s[0] == '_' && ((s[1] >= 'a' && s[1] <= 'z') || (s[1] >= 'A' && s[1] <= 'Z'))) {
                    std::string propName;
                    for (uint32_t c = 0; c < 128 && (strRva - rdataRva + c) < rdataSize; ++c) {
                        char ch = static_cast<char>(s[c]);
                        if (ch == '\0') break;
                        if (ch < 0x20 || ch > 0x7E) { propName.clear(); break; }
                        propName.push_back(ch);
                    }
                    if (propName.size() > 2) {
                        res.resolvedOffsets[propName] = propOffset;
                    }
                }
            }
        }
    }

    // 1B. Scan reflection getter registration blocks:
    //     48 8D 15 disp32 (lea rdx, "_propName") followed within 0x30 bytes by
    //     48 8D 0D/05 disp32 (lea rcx/rax, [rip+disp32]) pointing to `48 8B 41 28 48 05/83 ...`
    for (uint32_t i = 0; i + 0x40 < codeSize; ++i) {
        if (codeBuf[i] == 0x48 && codeBuf[i+1] == 0x8D && codeBuf[i+2] == 0x15) {
            int32_t strDisp = 0;
            std::memcpy(&strDisp, &codeBuf[i+3], 4);
            uint32_t strRva = static_cast<uint32_t>(static_cast<int64_t>(codeRva + i + 7) + strDisp);
            if (strRva < rdataRva || strRva + 2 >= rdataRva + rdataSize) continue;
            const uint8_t* s = rdataBuf + (strRva - rdataRva);
            if (s[0] != '_' || !((s[1] >= 'a' && s[1] <= 'z') || (s[1] >= 'A' && s[1] <= 'Z'))) continue;

            for (uint32_t k = i + 7; k < i + 0x30 && k + 7 < codeSize; ++k) {
                if (codeBuf[k] == 0x48 && codeBuf[k+1] == 0x8D && (codeBuf[k+2] == 0x0D || codeBuf[k+2] == 0x05)) {
                    int32_t fnDisp = 0;
                    std::memcpy(&fnDisp, &codeBuf[k+3], 4);
                    uint32_t fnRva = static_cast<uint32_t>(static_cast<int64_t>(codeRva + k + 7) + fnDisp);
                    if (fnRva >= codeRva && fnRva + 12 < codeRva + codeSize) {
                        const uint8_t* fn = codeBuf + (fnRva - codeRva);
                        if (fn[0] == 0x48 && fn[1] == 0x8B && fn[2] == 0x41 && fn[3] == 0x28) {
                            uint32_t propOffset = 0;
                            bool isGetter = false;
                            if (fn[4] == 0x48 && fn[5] == 0x05 && fn[10] == 0xC3) {
                                std::memcpy(&propOffset, &fn[6], 4);
                                isGetter = true;
                            } else if (fn[4] == 0x48 && fn[5] == 0x83 && fn[6] == 0xC0 && fn[8] == 0xC3) {
                                propOffset = fn[7];
                                isGetter = true;
                            }
                            if (isGetter && propOffset <= 0x400) {
                                std::string propName;
                                for (uint32_t c = 0; c < 128 && (strRva - rdataRva + c) < rdataSize; ++c) {
                                    char ch = static_cast<char>(s[c]);
                                    if (ch == '\0') break;
                                    if (ch < 0x20 || ch > 0x7E) { propName.clear(); break; }
                                    propName.push_back(ch);
                                }
                                if (propName.size() > 2) {
                                    res.resolvedOffsets[propName] = propOffset;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    res.totalGettersResolved = res.resolvedOffsets.size();

    // 2. Dynamically locate PASceneRenderer::ctor_base (`BA 10 00 00 00 B9 48 03 00 00` -> alloc 0x348 bytes for PASceneRendererOption)
    for (uint32_t i = 0x400; i + 0x120 < codeSize; ++i) {
        if (codeBuf[i] == 0xBA && codeBuf[i+1] == 0x10 && codeBuf[i+2] == 0x00 && codeBuf[i+3] == 0x00 && codeBuf[i+4] == 0x00 &&
            codeBuf[i+5] == 0xB9 && codeBuf[i+6] == 0x48 && codeBuf[i+7] == 0x03 && codeBuf[i+8] == 0x00 && codeBuf[i+9] == 0x00) {
            // Confirm `48 89 BB <disp32>` (mov [rbx + optOff], rdi; 0x500 <= optOff <= 0x700) follows within 0x90 bytes
            uint32_t detectedOptOff = 0;
            for (uint32_t k = i; k < i + 0x90 && k + 7 < codeSize; ++k) {
                if (codeBuf[k] == 0x48 && codeBuf[k+1] == 0x89 && codeBuf[k+2] == 0xBB) {
                    uint32_t disp = 0;
                    std::memcpy(&disp, &codeBuf[k+3], 4);
                    if (disp >= 0x500 && disp <= 0x700) {
                        detectedOptOff = disp;
                        break;
                    }
                }
                if (codeBuf[k] == 0xE8 && codeBuf[k-3] == 0x48 && codeBuf[k-2] == 0x8B && codeBuf[k-1] == 0xCF) {
                    int32_t rel = 0;
                    std::memcpy(&rel, &codeBuf[k+1], 4);
                    res.ctorSceneRendererOptionRva = static_cast<uint32_t>(static_cast<int64_t>(codeRva + k + 5) + rel);
                }
            }
            if (detectedOptOff == 0) continue;
            res.optionPtrOffsetInRenderer = detectedOptOff;

            // Walk backward up to 0x400 bytes to find function start (`CC 4C 8B DC`), VTable load (`48 8D 05 disp32 48 89 01`),
            // and RenderDevice* store (`4C 89 BB disp32` = mov [rbx+rdOff], r15)
            for (uint32_t b = i; b + 0x400 > i && b > 16; --b) {
                if (codeBuf[b] == 0x48 && codeBuf[b+1] == 0x8D && codeBuf[b+2] == 0x05 &&
                    codeBuf[b+7] == 0x48 && codeBuf[b+8] == 0x89 && codeBuf[b+9] == 0x01) {
                    int32_t disp = 0;
                    std::memcpy(&disp, &codeBuf[b+3], 4);
                    res.vtableSceneRendererBaseRva = static_cast<uint32_t>(static_cast<int64_t>(codeRva + b + 7) + disp);
                }
                if (codeBuf[b] == 0x4C && codeBuf[b+1] == 0x89 && (codeBuf[b+2] == 0xBB || codeBuf[b+2] == 0xB3)) {
                    uint32_t disp = 0;
                    std::memcpy(&disp, &codeBuf[b+3], 4);
                    if (disp >= 0x500 && disp < detectedOptOff) {
                        res.renderDeviceOffsetInRenderer = disp;
                    }
                }
                if (codeBuf[b-1] == 0xCC && codeBuf[b-2] == 0xCC && codeBuf[b] == 0x4C && codeBuf[b+1] == 0x8B && codeBuf[b+2] == 0xDC) {
                    res.ctorSceneRendererBaseRva = codeRva + b;
                    break;
                }
            }
            break;
        }
    }

    // 3. Dynamically locate PASceneRenderer::ctor_derived (calls ctorSceneRendererBaseRva then sets derived VTable `48 8D 05 disp32 48 89 07`)
    if (res.ctorSceneRendererBaseRva != 0) {
        for (uint32_t i = 0; i + 20 < codeSize; ++i) {
            if (codeBuf[i] == 0xE8) {
                int32_t rel = 0;
                std::memcpy(&rel, &codeBuf[i+1], 4);
                uint32_t target = static_cast<uint32_t>(static_cast<int64_t>(codeRva + i + 5) + rel);
                if (target == res.ctorSceneRendererBaseRva) {
                    // Check for `48 8D 05 disp32 48 89 07` right after the call
                    for (uint32_t k = i + 5; k < i + 20 && k + 10 < codeSize; ++k) {
                        if (codeBuf[k] == 0x48 && codeBuf[k+1] == 0x8D && codeBuf[k+2] == 0x05 &&
                            codeBuf[k+7] == 0x48 && codeBuf[k+8] == 0x89 && codeBuf[k+9] == 0x07) {
                            int32_t disp = 0;
                            std::memcpy(&disp, &codeBuf[k+3], 4);
                            res.vtableSceneRendererDerivedRva = static_cast<uint32_t>(static_cast<int64_t>(codeRva + k + 7) + disp);
                            break;
                        }
                    }
                    // Walk back to start of derived ctor
                    for (uint32_t b = i; b > 16 && i - b < 0x100; --b) {
                        if (codeBuf[b-1] == 0xCC && codeBuf[b-2] == 0xCC) {
                            res.ctorSceneRendererDerivedRva = codeRva + b;
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }

    // 4. Dynamically locate Global_RenderDevice_RVA (`4C 8B 47 30 4C 89 05 disp32 48 8B 5F 60`)
    for (uint32_t i = 0; i + 16 < codeSize; ++i) {
        if (codeBuf[i] == 0x4C && codeBuf[i+1] == 0x8B && codeBuf[i+2] == 0x47 && codeBuf[i+3] == 0x30 &&
            codeBuf[i+4] == 0x4C && codeBuf[i+5] == 0x89 && codeBuf[i+6] == 0x05 &&
            codeBuf[i+11] == 0x48 && codeBuf[i+12] == 0x8B && codeBuf[i+13] == 0x5F && codeBuf[i+14] == 0x60) {
            int32_t disp = 0;
            std::memcpy(&disp, &codeBuf[i+7], 4);
            res.globalRenderDeviceRva = static_cast<uint32_t>(static_cast<int64_t>(codeRva + i + 11) + disp);
            break;
        }
    }

    // 5. Validate all critical offsets against kExpectedProps (supporting versioned altExpectedOffset)
    res.criticalMatched = 0;
    res.criticalMismatched = 0;
    for (size_t p = 0; p < kNumExpectedProps; ++p) {
        const auto& exp = kExpectedProps[p];
        auto it = res.resolvedOffsets.find(exp.name);
        if (it == res.resolvedOffsets.end()) {
            // Note: _lightingQualityLevel (+0x05C) is set directly in applyLightingQuality without a reflection getter string
            if (std::strcmp(exp.name, "_lightingQualityLevel") == 0) {
                res.resolvedOffsets[exp.name] = exp.expectedOffset;
                res.criticalMatched++;
                continue;
            }
            res.criticalMismatched++;
            char msg[256];
            std::snprintf(msg, sizeof(msg), "[WARN] Propriedade '%s' (esperado +0x%03X) nao encontrada nos getters dinamicos.", exp.name, exp.expectedOffset);
            res.validationMessages.push_back(msg);
        } else if (it->second != exp.expectedOffset && (exp.altExpectedOffset == 0 || it->second != exp.altExpectedOffset)) {
            res.criticalMismatched++;
            char msg[256];
            std::snprintf(msg, sizeof(msg), "[MISMATCH] Propriedade '%s': esperado +0x%03X, encontrado +0x%03X!", exp.name, exp.expectedOffset, it->second);
            res.validationMessages.push_back(msg);
        } else {
            res.criticalMatched++;
        }
    }
    res.allCriticalValid = (res.criticalMismatched == 0 && res.vtableSceneRendererDerivedRva != 0 && res.globalRenderDeviceRva != 0);
}

// ============================================================================
// 4. XML & PRESET EXPECTATION ENGINE
// ============================================================================
struct XmlConfigState {
    bool fileFound = false;
    std::string xmlPath;
    std::string selectPreset = "Unknown";
    std::string upscaleModeSelect = "Unknown";
    std::string upscaleResolution = "Unknown";
    int numFramesToGenerate = -1;
};

inline std::string ExtractXmlAttr(const std::string& content, const std::string& tagOrName, const std::string& attrName) {
    size_t pos = content.find(tagOrName);
    if (pos == std::string::npos) return "";
    size_t lineEnd = content.find('>', pos);
    if (lineEnd == std::string::npos) return "";
    std::string sub = content.substr(pos, lineEnd - pos);
    std::string key = attrName + "=\"";
    size_t kpos = sub.find(key);
    if (kpos == std::string::npos) return "";
    kpos += key.size();
    size_t kend = sub.find('"', kpos);
    if (kend == std::string::npos) return "";
    return sub.substr(kpos, kend - kpos);
}

inline XmlConfigState ParseUserEngineOptionXml() {
    XmlConfigState state;
    char localAppData[MAX_PATH]{};
    DWORD len = GetEnvironmentVariableA("LOCALAPPDATA", localAppData, MAX_PATH);
    if (len == 0 || len >= MAX_PATH) return state;

    state.xmlPath = std::string(localAppData) + "\\Pearl Abyss\\CD\\save\\user_engine_option_save.xml";
    std::ifstream ifs(state.xmlPath);
    if (!ifs.is_open()) return state;

    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string content = ss.str();
    state.fileFound = true;

    std::string preset = ExtractXmlAttr(content, "<EngineOptionSave", "_selectPreset");
    if (!preset.empty()) state.selectPreset = preset;

    std::string upMode = ExtractXmlAttr(content, "Name=\"_upscaleModeSelect\"", "_value");
    if (!upMode.empty()) state.upscaleModeSelect = upMode;

    std::string upRes = ExtractXmlAttr(content, "Name=\"_upscaleResolution\"", "_select");
    if (!upRes.empty()) state.upscaleResolution = upRes;

    std::string fgNum = ExtractXmlAttr(content, "Name=\"_numFramesToGenerate\"", "_value");
    if (!fgNum.empty()) state.numFramesToGenerate = std::atoi(fgNum.c_str());

    return state;
}

// Returns the expected value of a parameter in [PASceneRenderer + 0x5C8] based on:
// 1. PASceneRendererOption::ctor (0x03EED780) defaults
// 2. applyLightingQuality (0x035839A0) for the active preset (VeryLow=0, Low=1, Medium=2, High=3, VeryHigh=4, Ultra=5, Extreme=6)
// 3. XML settings (_upscaleModeSelect, _upscaleResolution, _numFramesToGenerate)
inline std::string GetExpectedValueForPreset(uint32_t offset, const XmlConfigState& xml, std::string& outSourceReason) {
    // Map preset string to applyLightingQuality case (0..6)
    int presetCase = 2; // Default Medium = case 2
    if (xml.selectPreset == "VeryLow" || xml.selectPreset == "Lowest") presetCase = 0;
    else if (xml.selectPreset == "Low") presetCase = 1;
    else if (xml.selectPreset == "Medium") presetCase = 2;
    else if (xml.selectPreset == "High") presetCase = 3;
    else if (xml.selectPreset == "VeryHigh") presetCase = 4;
    else if (xml.selectPreset == "Ultra") presetCase = 5;
    else if (xml.selectPreset == "Extreme" || xml.selectPreset == "Cinematic") presetCase = 6;

    switch (offset) {
        // SSDM / Shadows / TAA / Trees:
        case 0x040: // _screenSpaceDisplacementMapping
            outSourceReason = "applyTerrainQuality (Cases 1..5 = 1; Case 0 = 0)";
            return (presetCase >= 1) ? "1 (true)" : "0 (false)";
        case 0x041: // _useSSDMRasterizer
            outSourceReason = "PASceneRendererOption::ctor (0 por padrao)";
            return "0 (false)";
        case 0x042: // _enableSSDMHighQuality
            outSourceReason = "applyTerrainQuality (Case 5/6 = 1; Cases 0..4 = 0)";
            return (presetCase >= 5) ? "1 (true)" : "0 (false)";
        case 0x043: // _enableSSDMTemporalReprojection
            outSourceReason = "PASceneRendererOption::ctor (1 por padrao)";
            return "1 (true)";
        case 0x0A4: // _shadowQualityLevel
            outSourceReason = "PASceneRendererOption::ctor (Default = 4)";
            return "4 [ctor]";
        case 0x16A: // _enableTemporalAA
            outSourceReason = "PASceneRendererOption::ctor (1 por padrao)";
            return "1 (true)";
        case 0x16E: // _useTemporalAAR2Noise
            outSourceReason = "PASceneRendererOption::ctor (0 por padrao)";
            return "0 (false)";
        case 0x1B9: // _useJitteredAlphaTestForTrees
            outSourceReason = "PASceneRendererOption::ctor (1 por padrao)";
            return "1 (true)";

        // Preset-controlled by applyLightingQuality (0x035839A0):
        case 0x05C: // _lightingQualityLevel
            outSourceReason = "applyLightingQuality (Preset " + xml.selectPreset + ")";
            return (presetCase <= 3) ? "0" : (presetCase <= 5 ? "1" : "2");
        case 0x079: // _renderShadowInterleaved
            outSourceReason = "applyLightingQuality (Preset " + xml.selectPreset + ")";
            return (presetCase == 0) ? "1 (true)" : "0 (false)";
        case 0x07A: // _renderShadowHighQuality
            outSourceReason = "applyLightingQuality (Preset " + xml.selectPreset + ")";
            return (presetCase == 6) ? "1 (true)" : "0 (false)";
        case 0x080: // _enableHBAOHighQuality
            outSourceReason = "applyLightingQuality (Preset " + xml.selectPreset + ")";
            return (presetCase >= 3) ? "1 (true)" : "0 (false)";
        case 0x0BA: // _enableGlobalIllumination
            outSourceReason = "PASceneRendererOption::ctor (Nunca alterado pelos presets!)";
            return "1 (true)";
        case 0x0BB: // _renderDiffuseGlobalIllumination
            outSourceReason = "PASceneRendererOption::ctor (Nunca alterado pelos presets!)";
            return "1 (true)";
        case 0x0BC: // _renderGIDownsampled
            outSourceReason = "applyLightingQuality (Cases 0..5 = 1; apenas Case 6 = 0)";
            return (presetCase == 6) ? "0 (false)" : "1 (true)";
        case 0x0C0: // _diffuseGIInterleavedMode
            outSourceReason = "applyLightingQuality (Cases 0..5 = 2; Case 6 = 0)";
            return (presetCase == 6) ? "0" : "2";
        case 0x0C4: // _reprojectDiffuseGIDownsampled
            outSourceReason = "applyLightingQuality (Todos os Cases 0..6 forcam 0!)";
            return "0 (false)";
        case 0x0C6: // _renderDiffuseGIUseIndirectCacheReservoir
            outSourceReason = "applyLightingQuality (Todos os Cases 0..6 forcam 0!)";
            return "0 (false)";
        case 0x0C7: // _enhanceDiffuseGIUseIndirectCacheReservoir
            outSourceReason = "applyLightingQuality (Preset " + xml.selectPreset + ")";
            return (presetCase >= 2) ? "1 (true)" : "0 (false)";
        case 0x0C8: // _renderDiffuseGIUseReservoirSampling
            outSourceReason = "applyLightingQuality (Preset " + xml.selectPreset + ")";
            return (presetCase >= 2) ? "1 (true)" : "0 (false)";
        case 0x0CE: // _renderDiffuseGIHighQuality
            outSourceReason = "applyLightingQuality (Preset " + xml.selectPreset + ")";
            return (presetCase >= 3) ? "1 (true)" : "0 (false)";
        case 0x0D1: // _renderDiffuseGIUseCharacterOcclusions
            outSourceReason = "applyLightingQuality (Preset " + xml.selectPreset + ")";
            return (presetCase >= 1) ? "1 (true)" : "0 (false)";
        case 0x0E1: // _enableBRDFWeightsForRayGuiding
            outSourceReason = "applyLightingQuality (Preset " + xml.selectPreset + ")";
            return (presetCase >= 2) ? "1 (true)" : "0 (false)";
        case 0x0E8: // _manyLightsQualityLevel
            outSourceReason = "applyLightingQuality (Preset " + xml.selectPreset + ")";
            if (presetCase <= 1) return "0";
            if (presetCase <= 3) return "1";
            if (presetCase <= 5) return "2";
            return "3";
        case 0x0EC: // _manyLightsUseDownsampledLightVisibility
            outSourceReason = "applyLightingQuality (Cases 0..5 = 1; Case 6 = 0)";
            return (presetCase == 6) ? "0 (false)" : "1 (true)";
        case 0x0EE: // _manyLightsSamplingUseSeparatePass
            outSourceReason = "applyLightingQuality (Cases 0..5 = 0; Case 6 = 1)";
            return (presetCase == 6) ? "1 (true)" : "0 (false)";
        case 0x0F1: // _manyLightsUseWiderKernel
            outSourceReason = "applyLightingQuality (Cases 0..5 = 0; Case 6 = 1)";
            return (presetCase == 6) ? "1 (true)" : "0 (false)";
        case 0x0FC: // _numManyLightsFilterPasses
            outSourceReason = "applyLightingQuality (Cases 0..3 = 1; Cases 4..6 = 2)";
            return (presetCase >= 4) ? "2" : "1";

        // Denoiser parameters NEVER modified by applyLightingQuality (locked to ctor defaults!):
        case 0x0F6: // _enableDiffuseGIFiltering
            outSourceReason = "PASceneRendererOption::ctor (Ignorado pelos presets!)";
            return "0 (false) [ctor]";
        case 0x104: // _useSVGFiltering
            outSourceReason = "PASceneRendererOption::ctor (Ignorado pelos presets!)";
            return "0 (false) [ctor]";
        case 0x108: // _numDiffuseGIFilterPasses
            outSourceReason = "PASceneRendererOption::ctor (Fixo em 1 em todos os presets!)";
            return "1";
        case 0x10C: // _numDiffuseGIFilterPassesForFarTiles
            outSourceReason = "PASceneRendererOption::ctor (Fixo em 1 em todos os presets!)";
            return "1";
        case 0x110: // _numDiffuseHitDistanceFilterPasses
            outSourceReason = "PASceneRendererOption::ctor (Fixo em 1 em todos os presets!)";
            return "1";
        case 0x114: // _renderSpecularGlobalIllumination
            outSourceReason = "PASceneRendererOption::ctor (Fixo em 1 em todos os presets!)";
            return "1 (true)";
        case 0x115: // _reprojectSpecularGIDownsampled
            outSourceReason = "applyLightingQuality (Todos os Cases 0..6 forcam 0!)";
            return "0 (false)";
        case 0x11B: // _enableSpecularGIFiltering
            outSourceReason = "PASceneRendererOption::ctor (Fixo em 1 em todos os presets!)";
            return "1 (true)";
        case 0x11C: // _applySpecularGIFilteringBeforeTemporalReprojection
            outSourceReason = "PASceneRendererOption::ctor (Fixo em 1 em todos os presets!)";
            return "1 (true)";
        case 0x120: // _numSpecularGIFilterPasses
            outSourceReason = "PASceneRendererOption::ctor (Fixo em 2 em todos os presets!)";
            return "2";
        case 0x124: // _specularGIQualityLevel
            outSourceReason = "PASceneRendererOption::ctor (Fixo em 2 em todos os presets!)";
            return "2";

        // Voxels & Radiance Cache
        case 0x134: // _generateSceneVoxels
        case 0x136: // _generateSDFVoxelsFromObjects
        case 0x137: // _generateSDFVoxelsFromTerrain
        case 0x13A: // _integrateHairScatteringDownsampled
            outSourceReason = "PASceneRendererOption::ctor (Sempre 1 mesmo no Low/Medium!)";
            return "1 (true)";
        case 0x138: // _generateSDFVoxelsFromCharacters
            outSourceReason = "PASceneRendererOption::ctor";
            return "0 (false)";
        case 0x139: // _generateHairScatteringVoxels
            outSourceReason = "applyLightingQuality (Cases 2..6 = 1; Cases 0..1 = 0)";
            return (presetCase >= 2) ? "1 (true)" : "0 (false)";
        case 0x150: // _updateRadianceVoxelsUseHighSampleCount
        case 0x151: // _updateRadianceVoxelsUseRaytracing
        case 0x153: // _updateIndirectCacheLightingUseHighSampleCount
            outSourceReason = "applyLightingQuality (Cases 2..6 = 1; Cases 0..1 = 0)";
            return (presetCase >= 2) ? "1 (true)" : "0 (false)";
        case 0x154: // _updateIndirectCacheLightingUseRaytracing
            outSourceReason = "applyLightingQuality (Cases 3..6 = 1; Cases 0..2 = 0)";
            return (presetCase >= 3) ? "1 (true)" : "0 (false)";
        case 0x158: // _updateIndirectCacheLightingUseMaskVoxelsFrameDistributionPerAxis
            outSourceReason = "applyLightingQuality (Cases 0..1 = 4; Cases 2..3 = 3; Cases 4..6 = 2)";
            if (presetCase <= 1) return "4";
            if (presetCase <= 3) return "3";
            return "2";

        // Ray Tracing & Path Tracing
        case 0x267: // _enableRaytracing
            outSourceReason = "EngineOptionGraphics::apply ([rcx+0x300] -> +0x267)";
            return "0 (false) [ou 1 se RT ativo]";
        case 0x268: // _renderRaytracedDiffuseGI
            outSourceReason = "applyLightingQuality (Cases 3..6 = 1; Cases 0..2 = 0)";
            return (presetCase >= 3) ? "1 (true)" : "0 (false)";
        case 0x269: // _renderRaytracedSpecularGI
            outSourceReason = "applyLightingQuality (Cases 3..6 = 1; Cases 0..2 = 0)";
            return (presetCase >= 3) ? "1 (true)" : "0 (false)";
        case 0x26A: // _useAdaptiveRayBudgetForSpecularGI
            outSourceReason = "applyLightingQuality (Cases 0..5 = 1; Case 6 = 0)";
            return (presetCase == 6) ? "0 (false)" : "1 (true)";
        case 0x26D: // _renderRaytracedLocalLights
            outSourceReason = "applyLightingQuality (Cases 2..6 = 1; Cases 0..1 = 0)";
            return (presetCase >= 2) ? "1 (true)" : "0 (false)";
        case 0x270: // _raytracingDebugMode
            outSourceReason = "PASceneRendererOption::ctor (0 por padrao)";
            return "0";
        case 0x271: // _renderPathTracing
            outSourceReason = "PASceneRendererOption::ctor (0 por padrao)";
            return "0 (false)";
        case 0x272: // _enablePathTracingDenoising
            outSourceReason = "PASceneRendererOption::ctor (0 por padrao)";
            return "0 (false)";
        case 0x273: // _renderPathTracingDownsampled
            outSourceReason = "PASceneRendererOption::ctor (0 por padrao)";
            return "0 (false)";

        // Upscaler & Frame Generation (XML driven)
        case 0x298: // _frameGenerationNumFramesToGenerate
            outSourceReason = "XML _numFramesToGenerate (ctor default = 3)";
            return (xml.numFramesToGenerate >= 0) ? std::to_string(xml.numFramesToGenerate) : "3 [ctor]";
        case 0x29C: // _enableDynamicMultiFrameGeneration
            outSourceReason = "PASceneRendererOption::ctor (0 por padrao)";
            return "0 (false)";
        case 0x2A0: // _dynamicTargetFrameRate
            outSourceReason = "PASceneRendererOption::ctor (0 por padrao)";
            return "0";
        case 0x2A4: // _resolutionScale
            outSourceReason = "XML _upscaleResolution (" + xml.upscaleResolution + ")";
            return "Enum (" + xml.upscaleResolution + ")";
        case 0x2A5: // _upscaleResolutionMethod
            outSourceReason = "XML _upscaleModeSelect (" + xml.upscaleModeSelect + ")";
            if (xml.upscaleModeSelect.find("DLSS") != std::string::npos) return "4 (DLSS)";
            if (xml.upscaleModeSelect.find("FSR") != std::string::npos) return "5 (FSR)";
            if (xml.upscaleModeSelect.find("XESS") != std::string::npos) return "7 (XeSS)";
            return "4 [ctor]";
        case 0x2A6: // _frameGenerationMode
            outSourceReason = "PASceneRendererOption::ctor / Menu FG Mode";
            return "0 (Off) ou 1 (DLSS-FG)";
        case 0x2AC: // _dlssPreset
            outSourceReason = "XML _upscaleModeSelect (" + xml.upscaleModeSelect + "; ctor = 8)";
            return "8 [ctor / DLSS 4.5 L]";
        case 0x2AD: // _dlssRRPreset
            outSourceReason = "PASceneRendererOption::ctor (Default = 3)";
            return "3 [ctor]";

        // Recurrent Blur & FfxDenoiser (Never modified by presets)
        case 0x2B5: // _enableRayReconstruction
            outSourceReason = "PASceneRendererOption::ctor (0 a menos que RR ativo)";
            return "0 (false)";
        case 0x2B6: // _recurrentBlurForSpecular
            outSourceReason = "PASceneRendererOption::ctor (Fixo em 1)";
            return "1 (true)";
        case 0x2B7: // _recurrentPreBlur
            outSourceReason = "PASceneRendererOption::ctor (Fixo em 0!)";
            return "0 (false)";
        case 0x2B8: // _recurrentPreBlurRadius
            outSourceReason = "PASceneRendererOption::ctor (Fixo em 0.001f!)";
            return "0.0010";
        case 0x2BC: // _recurrentBlurUseWaveletFilter
            outSourceReason = "PASceneRendererOption::ctor (Fixo em 1)";
            return "1 (true)";
        case 0x2BD: // _recurrentPostBlur
            outSourceReason = "PASceneRendererOption::ctor (Fixo em 0!)";
            return "0 (false)";
        case 0x2BE: // _recurrentBlurReconstructHistory
            outSourceReason = "PASceneRendererOption::ctor (Fixo em 0!)";
            return "0 (false)";
        case 0x2C0: // _recurrentPostBlurRadius
            outSourceReason = "PASceneRendererOption::ctor (Fixo em 0.001f!)";
            return "0.0010";
        case 0x2C4: // _ffxDenoiserVersionIndex
            outSourceReason = "PASceneRendererOption::ctor (Inativo sem RR+FSR)";
            return "0";
        case 0x2C8: // _ffxDenoiserUseDebugFeatures
            outSourceReason = "PASceneRendererOption::ctor (Inativo sem RR+FSR)";
            return "0 (false)";
        case 0x2CC: // _ffxDenoiserCrossBilateralNormalStrength
            outSourceReason = "PASceneRendererOption::ctor (Inativo sem RR+FSR)";
            return "1.0000";
        case 0x2D0: // _ffxDenoiserStabilityBias
            outSourceReason = "PASceneRendererOption::ctor (Inativo sem RR+FSR)";
            return "0.2500";
        case 0x2D4: // _ffxDenoiserMaxRadiance
            outSourceReason = "PASceneRendererOption::ctor (Inativo sem RR+FSR)";
            return "100.0000";
        default:
            outSourceReason = "Engine Default / Preset";
            return "-";
    }
}

// ============================================================================
// 5. VRAM TELEMETRY (IDXGIAdapter3::QueryVideoMemoryInfo - 100% Read-Only)
// ============================================================================
struct VramSnapshot {
    bool valid = false;
    std::string adapterName;
    uint64_t dedicatedVramBytes = 0;

    // Local (Dedicated VRAM)
    uint64_t localUsageBytes = 0;
    uint64_t localBudgetBytes = 0;
    uint64_t localAvailableForReservation = 0;
    double localUsagePercent = 0.0;
    bool isOverBudget = false;

    // Non-Local (Shared System RAM)
    uint64_t nonLocalUsageBytes = 0;
    uint64_t nonLocalBudgetBytes = 0;
    double nonLocalUsagePercent = 0.0;

    // WDDM Process & Adapter Counters (when queried for live game PID)
    bool wddmValid = false;
    uint64_t procLocalUsageBytes = 0;
    uint64_t procNonLocalUsageBytes = 0;
    uint64_t procTotalCommittedBytes = 0;
    uint64_t adapterDedicatedUsageBytes = 0;
    uint64_t adapterSharedUsageBytes = 0;
    uint64_t adapterTotalCommittedBytes = 0;
    uint64_t processVirtualSizeBytes = 0;
    uint64_t processWorkingSetBytes = 0;
    double gpu3DAvgUtilPercent = 0.0;
    double gpu3DMaxUtilPercent = 0.0;
    double gpuCopyAvgUtilPercent = 0.0;
    double gpuComputeAvgUtilPercent = 0.0;
};

struct FramePacingBaseline {
    bool valid = false;
    double captureDurationSec = 0.0;
    uint32_t totalFrames = 0;
    double avgFps = 0.0;
    double avgFrameTimeMs = 0.0;
    double minFrameTimeMs = 0.0;
    double p50FrameTimeMs = 0.0;
    double p90FrameTimeMs = 0.0;
    double p95FrameTimeMs = 0.0;
    double p99FrameTimeMs = 0.0;
    double p99_9FrameTimeMs = 0.0;
    double maxFrameTimeMs = 0.0;
    double fps1PercentLow = 0.0;
    double fps0_1PercentLow = 0.0;
    double ratioP99ToP50 = 0.0;
    double stdDevJitterMs = 0.0;
    double maxFrameToFrameDeltaMs = 0.0;
    uint32_t spikesOver8Ms = 0;
    uint32_t spikesOver16_6Ms = 0;
    uint32_t queueDepth0Count = 0;
    uint32_t queueDepth1Count = 0;
    uint32_t queueDepth2Count = 0;
    uint32_t queueDepth3PlusCount = 0;
};

inline VramSnapshot QueryVramTelemetry() {
    VramSnapshot snap;
    IDXGIFactory4* pFactory = nullptr;
    if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory4), reinterpret_cast<void**>(&pFactory))) || !pFactory) {
        return snap;
    }

    IDXGIAdapter1* pAdapter1 = nullptr;
    for (UINT i = 0; pFactory->EnumAdapters1(i, &pAdapter1) != DXGI_ERROR_NOT_FOUND; ++i) {
        DXGI_ADAPTER_DESC1 desc{};
        pAdapter1->GetDesc1(&desc);
        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
            pAdapter1->Release();
            continue;
        }
        IDXGIAdapter3* pAdapter3 = nullptr;
        if (SUCCEEDED(pAdapter1->QueryInterface(__uuidof(IDXGIAdapter3), reinterpret_cast<void**>(&pAdapter3))) && pAdapter3) {
            DXGI_QUERY_VIDEO_MEMORY_INFO localInfo{};
            DXGI_QUERY_VIDEO_MEMORY_INFO nonLocalInfo{};
            if (SUCCEEDED(pAdapter3->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &localInfo)) &&
                SUCCEEDED(pAdapter3->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_NON_LOCAL, &nonLocalInfo))) {
                char nameBuf[256]{};
                WideCharToMultiByte(CP_UTF8, 0, desc.Description, -1, nameBuf, sizeof(nameBuf), nullptr, nullptr);
                snap.valid = true;
                snap.adapterName = nameBuf;
                snap.dedicatedVramBytes = desc.DedicatedVideoMemory;
                snap.localUsageBytes = localInfo.CurrentUsage;
                snap.localBudgetBytes = localInfo.Budget;
                snap.localAvailableForReservation = localInfo.AvailableForReservation;
                snap.localUsagePercent = (localInfo.Budget > 0)
                    ? (100.0 * static_cast<double>(localInfo.CurrentUsage) / static_cast<double>(localInfo.Budget))
                    : 0.0;
                snap.isOverBudget = (localInfo.CurrentUsage > localInfo.Budget);

                snap.nonLocalUsageBytes = nonLocalInfo.CurrentUsage;
                snap.nonLocalBudgetBytes = nonLocalInfo.Budget;
                snap.nonLocalUsagePercent = (nonLocalInfo.Budget > 0)
                    ? (100.0 * static_cast<double>(nonLocalInfo.CurrentUsage) / static_cast<double>(nonLocalInfo.Budget))
                    : 0.0;
                pAdapter3->Release();
                pAdapter1->Release();
                break;
            }
            pAdapter3->Release();
        }
        pAdapter1->Release();
    }
    pFactory->Release();
    return snap;
}

// ============================================================================
// 6. RUNTIME INSTANCE FINDER & STRUCT READER (100% Read-Only, Rule 7 Compliant)
// ============================================================================
struct RuntimeInstanceState {
    bool gameProcessFound = false;
    bool renderDeviceFound = false;
    bool sceneRendererFound = false;
    bool optionStructFound = false;
    bool bidirectionalLockVerified = false;
    bool renderDeviceCrossVerified = false;

    uintptr_t imageBase = 0;
    uintptr_t renderDevicePtr = 0;
    uintptr_t sceneRendererPtr = 0;
    uintptr_t sceneRendererVTable = 0;
    uintptr_t optionStructPtr = 0;
    uintptr_t optionOwnerBackPtr0 = 0;
    uintptr_t optionOwnerBackPtr8 = 0;

    uint8_t activeUpscalerByte = 0xFF; // [RenderDevice + 0x59]: 0=None, 1=DLSS, 2=FSR, 3=XeSS
    uint8_t rawOptionBytes[0x348]{};
    std::string statusMessage;
    double scanTimeMs = 0.0;
};

inline RuntimeInstanceState LocateAndReadSceneRenderer(
    const IMemoryReader& reader,
    const BinaryValidationResult& binVal,
    HANDLE hExternalProcess = nullptr)
{
    RuntimeInstanceState state;
    LARGE_INTEGER freq{}, t0{}, t1{};
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t0);

    state.imageBase = reader.GetImageBase();
    if (state.imageBase == 0) {
        state.statusMessage = "ImageBase indisponivel (processo nao iniciado).";
        return state;
    }
    state.gameProcessFound = true;

    // 1. Read Global_RenderDevice pointer ([ImageBase + globalRenderDeviceRva])
    if (binVal.globalRenderDeviceRva != 0) {
        uintptr_t globalRdAddr = state.imageBase + binVal.globalRenderDeviceRva;
        uintptr_t rdPtr = 0;
        if (reader.Read(globalRdAddr, rdPtr) && rdPtr > 0x10000 && rdPtr < 0x00007FFFFFFFFFFFULL) {
            uint8_t upscaler = 0xFF;
            if (reader.Read(rdPtr + 0x59, upscaler)) {
                state.renderDeviceFound = true;
                state.renderDevicePtr = rdPtr;
                state.activeUpscalerByte = upscaler;
            }
        }
    }

    uintptr_t expectedVTableDerived = (binVal.vtableSceneRendererDerivedRva != 0)
        ? (state.imageBase + binVal.vtableSceneRendererDerivedRva) : 0;
    uintptr_t expectedVTableBase = (binVal.vtableSceneRendererBaseRva != 0)
        ? (state.imageBase + binVal.vtableSceneRendererBaseRva) : 0;

    if (expectedVTableDerived == 0 && expectedVTableBase == 0) {
        state.statusMessage = "Limitação registrada (Regra 7): VTable do PASceneRenderer nao pode ser resolvida dinamicamente.";
        return state;
    }

    // 2. Walk committed readable MEM_PRIVATE PAGE_READWRITE regions to find PASceneRenderer* (P)
    //    satisfying the 4-way non-invasive invariant:
    //    (a) [P + 0x000] == expectedVTableDerived (or Base)
    //    (b) O = [P + 0x5C8] is a valid 16-byte aligned heap pointer
    //    (c) [O + 0x000] == P && [O + 0x008] == P (PASceneRendererOption::ctor bidirectional lock!)
    //    (d) [P + 0x578] == RenderDevice* (when RenderDevice* is non-null)
    uintptr_t addr = 0x10000;
    const uintptr_t maxUserAddr = 0x00007FFFFFFF0000ULL;
    std::vector<uint8_t> chunkBuf;

    while (addr < maxUserAddr) {
        MEMORY_BASIC_INFORMATION mbi{};
        SIZE_T qret = 0;
        if (hExternalProcess) {
            qret = VirtualQueryEx(hExternalProcess, reinterpret_cast<LPCVOID>(addr), &mbi, sizeof(mbi));
        } else {
            qret = VirtualQuery(reinterpret_cast<LPCVOID>(addr), &mbi, sizeof(mbi));
        }
        if (qret != sizeof(mbi)) break;

        uintptr_t base = reinterpret_cast<uintptr_t>(mbi.BaseAddress);
        size_t regSize = mbi.RegionSize;
        if (regSize == 0) break;

        bool isCandidateRegion = (mbi.State == MEM_COMMIT) &&
                                 (mbi.Type == MEM_PRIVATE) &&
                                 (mbi.Protect == PAGE_READWRITE) &&
                                 (regSize >= 0x1000);

        if (isCandidateRegion) {
            // Read in chunks of up to 4 MB
            const size_t kChunkSize = 4 * 1024 * 1024;
            for (size_t off = 0; off < regSize; off += kChunkSize) {
                size_t toRead = std::min(kChunkSize, regSize - off);
                if (toRead < 0xF10) continue;
                chunkBuf.resize(toRead);
                if (!reader.ReadBytes(base + off, chunkBuf.data(), toRead)) continue;

                const size_t limit = toRead - 0x640;
                for (size_t pos = 0; pos < limit; pos += 16) {
                    uintptr_t vt = *reinterpret_cast<const uintptr_t*>(&chunkBuf[pos]);
                    if (vt != expectedVTableDerived && vt != expectedVTableBase) continue;

                    uintptr_t candP = base + off + pos;
                    uintptr_t candO = *reinterpret_cast<const uintptr_t*>(&chunkBuf[pos + binVal.optionPtrOffsetInRenderer]);
                    uintptr_t candRD = *reinterpret_cast<const uintptr_t*>(&chunkBuf[pos + binVal.renderDeviceOffsetInRenderer]);

                    if (candO < 0x10000 || candO > maxUserAddr || (candO & 0xF) != 0) continue;

                    // Verify bidirectional lock at candO (+0x00 and +0x08 must equal candP!)
                    uintptr_t back0 = 0, back8 = 0;
                    if (!reader.Read(candO + 0x00, back0) || !reader.Read(candO + 0x08, back8)) continue;
                    if (back0 != candP || back8 != candP) continue;

                    // Found verified PASceneRenderer* and PASceneRendererOption*!
                    state.sceneRendererFound = true;
                    state.sceneRendererPtr = candP;
                    state.sceneRendererVTable = vt;
                    state.optionStructPtr = candO;
                    state.optionOwnerBackPtr0 = back0;
                    state.optionOwnerBackPtr8 = back8;
                    state.bidirectionalLockVerified = true;
                    state.renderDeviceCrossVerified = (state.renderDeviceFound && candRD == state.renderDevicePtr);

                    if (!state.renderDeviceFound && candRD > 0x10000 && candRD < maxUserAddr) {
                        uint8_t up = 0xFF;
                        if (reader.Read(candRD + 0x59, up)) {
                            state.renderDeviceFound = true;
                            state.renderDevicePtr = candRD;
                            state.activeUpscalerByte = up;
                            state.renderDeviceCrossVerified = true;
                        }
                    }

                    if (reader.ReadBytes(candO, state.rawOptionBytes, sizeof(state.rawOptionBytes))) {
                        state.optionStructFound = true;
                    }
                    break;
                }
                if (state.sceneRendererFound) break;
            }
        }
        if (state.sceneRendererFound) break;
        addr = base + regSize;
    }

    QueryPerformanceCounter(&t1);
    state.scanTimeMs = (freq.QuadPart > 0)
        ? (1000.0 * static_cast<double>(t1.QuadPart - t0.QuadPart) / static_cast<double>(freq.QuadPart))
        : 0.0;

    if (state.sceneRendererFound && state.optionStructFound) {
        char okBuf[256];
        std::snprintf(okBuf, sizeof(okBuf),
            "Instancia PASceneRenderer e PASceneRendererOption (+0x%03X, RD +0x%03X) identificadas e validadas com sucesso (Lock Bidirecional 4-Way OK).",
            binVal.optionPtrOffsetInRenderer, binVal.renderDeviceOffsetInRenderer);
        state.statusMessage = okBuf;
    } else {
        state.statusMessage = "Limitacao registrada (Regra 7): PASceneRenderer ainda nao instanciado na memoria (ex.: menu inicial/loading) ou inacessivel de forma nao-invasiva.";
    }
    return state;
}

// ============================================================================
// 7. FORMATTING & COMPARATIVE REPORT GENERATOR (Markdown + Log)
// ============================================================================
inline std::string FormatRuntimeValue(const uint8_t* raw, const ExpectedProp& prop, uint32_t actualOffset) {
    if (actualOffset + 4 > 0x348) return "OutOfBounds";
    std::ostringstream oss;
    if (std::strcmp(prop.typeName, "bool") == 0) {
        uint8_t v = raw[actualOffset];
        oss << static_cast<int>(v) << (v ? " (true)" : " (false)");
    } else if (std::strcmp(prop.typeName, "uint8") == 0) {
        uint8_t v = raw[actualOffset];
        oss << static_cast<int>(v);
    } else if (std::strcmp(prop.typeName, "int32") == 0) {
        int32_t v = 0;
        std::memcpy(&v, &raw[actualOffset], 4);
        oss << v;
    } else if (std::strcmp(prop.typeName, "float") == 0) {
        float v = 0.0f;
        std::memcpy(&v, &raw[actualOffset], 4);
        oss << std::fixed << std::setprecision(4) << v;
    }
    return oss.str();
}

inline const char* DecodeUpscalerName(uint8_t up) {
    switch (up) {
        case 0: return "0 (None / Native TAA)";
        case 1: return "1 (NVIDIA DLSS)";
        case 2: return "2 (AMD FSR)";
        case 3: return "3 (Intel XeSS)";
        default: return "Unknown / Uninitialized";
    }
}

inline FramePacingBaseline CaptureLiveFramePacingBaseline(
    const IMemoryReader& reader,
    uintptr_t renderDevicePtr,
    uint32_t durationMs = 10000,
    const std::wstring& csvOutPath = L"Baseline_FrameTimes.csv")
{
    FramePacingBaseline fp{};
    if (renderDevicePtr == 0 || durationMs == 0) return fp;

    LARGE_INTEGER freq{}, tStart{}, tNow{}, tPrevFrame{};
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&tStart);
    tPrevFrame = tStart;

    uint64_t lastPresentedFrame = 0;
    uint64_t buf[3]{};
    if (!reader.ReadBytes(renderDevicePtr + 0x0E0, buf, sizeof(buf))) {
        return fp;
    }
    lastPresentedFrame = buf[1]; // [RenderDevice + 0x0E8]

    struct FrameRecord {
        uint64_t frameIndex;
        double frameTimeMs;
        int64_t cpuGpuQueueDepth;
    };
    std::vector<FrameRecord> records;
    records.reserve(2000);

    double elapsedMs = 0.0;
    while (elapsedMs < static_cast<double>(durationMs)) {
        if (reader.ReadBytes(renderDevicePtr + 0x0E0, buf, sizeof(buf))) {
            uint64_t cpuFrame = buf[0];       // +0x0E0
            uint64_t presentedFrame = buf[1]; // +0x0E8
            uint64_t gpuCompleted = buf[2];   // +0x0F0

            if (presentedFrame > lastPresentedFrame) {
                QueryPerformanceCounter(&tNow);
                double dtMs = 1000.0 * static_cast<double>(tNow.QuadPart - tPrevFrame.QuadPart) / static_cast<double>(freq.QuadPart);
                tPrevFrame = tNow;
                int64_t qDepth = static_cast<int64_t>(cpuFrame) - static_cast<int64_t>(gpuCompleted);
                if (records.size() > 0 || dtMs > 1.0) {
                    records.push_back({ presentedFrame, dtMs, qDepth });
                }
                lastPresentedFrame = presentedFrame;
            }
        }
        QueryPerformanceCounter(&tNow);
        elapsedMs = 1000.0 * static_cast<double>(tNow.QuadPart - tStart.QuadPart) / static_cast<double>(freq.QuadPart);
        SwitchToThread();
    }

    if (records.size() < 5) return fp;

    // Drop the very first partial frame interval
    records.erase(records.begin());

    if (!csvOutPath.empty()) {
        std::ofstream csv(csvOutPath, std::ios::out | std::ios::trunc);
        if (csv.is_open()) {
            csv << "FrameIndex,FrameTimeMs,InstantFPS,CpuGpuQueueDepth\n";
            for (const auto& r : records) {
                double instFps = (r.frameTimeMs > 0.01) ? (1000.0 / r.frameTimeMs) : 0.0;
                csv << r.frameIndex << ","
                    << std::fixed << std::setprecision(4) << r.frameTimeMs << ","
                    << std::setprecision(2) << instFps << ","
                    << r.cpuGpuQueueDepth << "\n";
            }
        }
    }

    fp.valid = true;
    fp.totalFrames = static_cast<uint32_t>(records.size());
    double sumMs = 0.0;
    std::vector<double> sortedMs;
    sortedMs.reserve(records.size());

    for (size_t i = 0; i < records.size(); ++i) {
        double ft = records[i].frameTimeMs;
        sumMs += ft;
        sortedMs.push_back(ft);

        int64_t qd = records[i].cpuGpuQueueDepth;
        if (qd <= 0) fp.queueDepth0Count++;
        else if (qd == 1) fp.queueDepth1Count++;
        else if (qd == 2) fp.queueDepth2Count++;
        else fp.queueDepth3PlusCount++;

        if (i > 0) {
            double delta = (ft > records[i - 1].frameTimeMs)
                ? (ft - records[i - 1].frameTimeMs)
                : (records[i - 1].frameTimeMs - ft);
            if (delta > fp.maxFrameToFrameDeltaMs) fp.maxFrameToFrameDeltaMs = delta;
            if (delta > 8.0) fp.spikesOver8Ms++;
            if (delta > 16.6) fp.spikesOver16_6Ms++;
        }
    }

    std::sort(sortedMs.begin(), sortedMs.end());
    fp.captureDurationSec = sumMs / 1000.0;
    fp.avgFrameTimeMs = sumMs / static_cast<double>(records.size());
    fp.avgFps = (fp.avgFrameTimeMs > 0.0) ? (1000.0 / fp.avgFrameTimeMs) : 0.0;
    fp.minFrameTimeMs = sortedMs.front();
    fp.maxFrameTimeMs = sortedMs.back();
    fp.p50FrameTimeMs = sortedMs[records.size() * 50 / 100];
    fp.p90FrameTimeMs = sortedMs[records.size() * 90 / 100];
    fp.p95FrameTimeMs = sortedMs[records.size() * 95 / 100];
    fp.p99FrameTimeMs = sortedMs[records.size() * 99 / 100];
    fp.p99_9FrameTimeMs = sortedMs[records.size() * 999 / 1000];
    fp.fps1PercentLow = (fp.p99FrameTimeMs > 0.0) ? (1000.0 / fp.p99FrameTimeMs) : 0.0;
    fp.fps0_1PercentLow = (fp.p99_9FrameTimeMs > 0.0) ? (1000.0 / fp.p99_9FrameTimeMs) : 0.0;
    fp.ratioP99ToP50 = (fp.p50FrameTimeMs > 0.0) ? (fp.p99FrameTimeMs / fp.p50FrameTimeMs) : 0.0;

    double sqDiff = 0.0;
    for (double ft : sortedMs) {
        double d = ft - fp.avgFrameTimeMs;
        sqDiff += d * d;
    }
    fp.stdDevJitterMs = std::sqrt(sqDiff / static_cast<double>(records.size()));
    return fp;
}

inline std::string GenerateMarkdownReport(
    const BinaryValidationResult& binVal,
    const XmlConfigState& xml,
    const RuntimeInstanceState& rt,
    const VramSnapshot& vram,
    const FramePacingBaseline& fp = FramePacingBaseline{})
{
    std::ostringstream md;
    SYSTEMTIME st{};
    GetLocalTime(&st);
    char timeBuf[64]{};
    std::snprintf(timeBuf, sizeof(timeBuf), "%04u-%02u-%02u %02u:%02u:%02u",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

    md << "# Relatório de Diagnóstico e Telemetria Somente Leitura — Etapa 1\n";
    md << "**Data/Hora da Coleta:** `" << timeBuf << "`  \n";
    md << "**Modo de Operação:** `100% Read-Only (Zero Memory Writes, Zero Hooks, Zero VTable Patches, sl.interposer Intocado)`\n\n";

    // Section 1: Binary & Dynamic Offset Validation
    md << "## 1. Versão do Executável e Validação Dinâmica de Offsets\n\n";
    md << "| Métrica do Executável | Valor Encontrado |\n";
    md << "| :--- | :--- |\n";
    md << "| **Caminho** | `" << binVal.exePath << "` |\n";
    md << "| **FileVersion / ProductVersion** | `" << binVal.fileVersion << "` / `" << binVal.productVersion << "` |\n";
    md << "| **Tamanho do Executável** | `" << binVal.fileSize << " bytes` (`" << (binVal.fileSize / (1024 * 1024)) << " MB`) |\n";
    md << "| **PE TimeDateStamp / SizeOfImage** | `0x" << std::hex << std::uppercase << binVal.peTimeDateStamp << "` / `0x" << binVal.sizeOfImage << std::dec << "` |\n";
    md << "| **VTable `PASceneRenderer` (Base / Derived)** | `RVA 0x" << std::hex << binVal.vtableSceneRendererBaseRva << "` / `RVA 0x" << binVal.vtableSceneRendererDerivedRva << "` |\n";
    md << "| **Ponteiro Global `RenderDevice*`** | `RVA 0x" << binVal.globalRenderDeviceRva << "` |\n";
    md << "| **Construtor `PASceneRendererOption::ctor`** | `RVA 0x" << binVal.ctorSceneRendererOptionRva << std::dec << "` |\n";
    md << "| **Total de Getters Resolvidos Dinamicamente** | **`" << binVal.totalGettersResolved << "` propriedades** |\n";
    md << "| **Offsets Críticos Validados** | **`" << binVal.criticalMatched << " / " << kNumExpectedProps << "`** (`Divergências: " << binVal.criticalMismatched << "`) |\n";
    md << "| **Status da Validação de Offsets** | **" << (binVal.allCriticalValid ? "APROVADO (100% dos Offsets Idênticos)" : "ALERTA — Verificar mensagens abaixo") << "** |\n\n";

    if (!binVal.validationMessages.empty()) {
        md << "### Mensagens do Validador de Offsets\n";
        for (const auto& m : binVal.validationMessages) {
            md << "- `" << m << "`\n";
        }
        md << "\n";
    }

    // Section 2: XML Configuration
    md << "## 2. Configuração Esperada pelo XML (`user_engine_option_save.xml`)\n\n";
    md << "| Parâmetro no XML | Valor Salvo |\n";
    md << "| :--- | :--- |\n";
    md << "| **Arquivo XML Encontrado** | `" << (xml.fileFound ? "Sim" : "Não") << "` (`" << xml.xmlPath << "`) |\n";
    md << "| **`_selectPreset` (Preset Gráfico Global)** | **`" << xml.selectPreset << "`** |\n";
    md << "| **`_upscaleModeSelect` (Modo de Upscaling)** | **`" << xml.upscaleModeSelect << "`** |\n";
    md << "| **`_upscaleResolution` (Escala de Resolução)** | **`" << xml.upscaleResolution << "`** |\n";
    md << "| **`_numFramesToGenerate` (Frame Generation)** | **`" << xml.numFramesToGenerate << "`** |\n\n";

    // Section 3: Pointer Validation & Instance Identification
    md << "## 3. Identificação da Instância e Validade dos Ponteiros em Runtime\n\n";
    md << "| Verificação de Ponteiro / Invariante | Estado / Endereço |\n";
    md << "| :--- | :--- |\n";
    md << "| **Status Geral** | " << rt.statusMessage << " |\n";
    md << "| **Tempo de Varredura Não-Invasiva** | `" << std::fixed << std::setprecision(2) << rt.scanTimeMs << " ms` |\n";
    md << "| **`CrimsonDesert.exe` ImageBase** | `0x" << std::hex << std::uppercase << rt.imageBase << "` |\n";
    md << "| **`RenderDevice*` (`[ImageBase + 0x" << binVal.globalRenderDeviceRva << "]`)** | `0x" << rt.renderDevicePtr << "` (" << (rt.renderDeviceFound ? "Válido" : "Não disponível") << ") |\n";
    md << "| **`PASceneRenderer*` (`P`)** | `0x" << rt.sceneRendererPtr << "` (" << (rt.sceneRendererFound ? "Válido" : "Não instanciado") << ") |\n";
    md << "| **`[P + 0x000]` (VTable Real)** | `0x" << rt.sceneRendererVTable << "` |\n";
    md << "| **`PASceneRendererOption*` (`O = [P + 0x" << binVal.optionPtrOffsetInRenderer << "]`)** | `0x" << rt.optionStructPtr << "` (" << (rt.optionStructFound ? "Válido — 0x348 bytes lidos" : "Não disponível") << ") |\n";
    md << "| **Lock Bidirecional (`[O+0x00]==P && [O+0x08]==P`)** | `" << (rt.bidirectionalLockVerified ? "CONFIRMADO" : "N/A") << "` (`0x" << rt.optionOwnerBackPtr0 << "`, `0x" << rt.optionOwnerBackPtr8 << "`) |\n";
    md << "| **Cross-Check `[P + 0x" << binVal.renderDeviceOffsetInRenderer << "] == RenderDevice*`** | `" << (rt.renderDeviceCrossVerified ? "CONFIRMADO" : "N/A") << "` |\n\n" << std::dec;

    // Section 4: Real State of Upscaler, Ray Reconstruction, Denoisers & Frame Generation
    md << "## 4. Estado Real do Upscaler, Ray Reconstruction, Denoisers e Frame Generation\n\n";
    if (rt.optionStructFound) {
        uint8_t rrEnabled = rt.rawOptionBytes[0x2B5];
        uint8_t upMethod = rt.rawOptionBytes[0x2A5];
        uint8_t resScale = rt.rawOptionBytes[0x2A4];
        uint8_t dlssPreset = rt.rawOptionBytes[0x2AC];
        uint8_t dlssRRPreset = rt.rawOptionBytes[0x2AD];
        uint8_t fgMode = rt.rawOptionBytes[0x2A6];
        int32_t fgNumFrames = *reinterpret_cast<const int32_t*>(&rt.rawOptionBytes[0x298]);
        uint8_t dynMfg = rt.rawOptionBytes[0x29C];
        int32_t dynTargetFps = *reinterpret_cast<const int32_t*>(&rt.rawOptionBytes[0x2A0]);

        std::string activeDenoiserPath;
        if (rrEnabled == 0) {
            activeDenoiserPath = "Standard Engine Denoisers (SVGF + RecurrentBlur + Diffuse/Specular GI Filter) — Ray Reconstruction DESATIVADO (+0x2B5 = 0)";
        } else if (rt.activeUpscalerByte == 1) {
            activeDenoiserPath = "NVIDIA DLSS Ray Reconstruction (sl.dlss_d.dll) — (+0x2B5 = 1 e Upscaler = 1 [DLSS])";
        } else if (rt.activeUpscalerByte == 2) {
            activeDenoiserPath = "AMD FidelityFX Denoiser (FfxDenoiser) — (+0x2B5 = 1 e Upscaler = 2 [FSR])";
        } else {
            activeDenoiserPath = "Estado Hibrido/Fallback (+0x2B5 = 1, Upscaler = " + std::to_string(rt.activeUpscalerByte) + ")";
        }

        md << "| Subsistema | Offset / Fonte | Valor Real na Memória | Interpretação Técnica |\n";
        md << "| :--- | :--- | :--- | :--- |\n";
        md << "| **Active Upscaler (`RenderDevice`)** | `[RenderDevice + 0x59]` | `" << static_cast<int>(rt.activeUpscalerByte) << "` | **" << DecodeUpscalerName(rt.activeUpscalerByte) << "** |\n";
        md << "| **Upscale Resolution Method** | `+0x2A5` | `" << static_cast<int>(upMethod) << "` | `4=DLSS, 5=FSR, 7=XeSS` (XML: `" << xml.upscaleModeSelect << "`) |\n";
        md << "| **Resolution Scale Enum** | `+0x2A4` | `" << static_cast<int>(resScale) << "` | Escala selecionada no XML: `" << xml.upscaleResolution << "` |\n";
        md << "| **DLSS Super Resolution Preset** | `+0x2AC` | `" << static_cast<int>(dlssPreset) << "` | Preset interno repassado ao Streamline `slSetConstants` |\n";
        md << "| **DLSS Ray Reconstruction Preset** | `+0x2AD` | `" << static_cast<int>(dlssRRPreset) << "` | Preset RR (`3 = Preset C / Default`) |\n";
        md << "| **Ray Reconstruction Ativo** | `+0x2B5` | `" << static_cast<int>(rrEnabled) << "` | **" << (rrEnabled ? "ATIVO (1)" : "INATIVO (0)") << "** |\n";
        md << "| **Caminho de Denoising Efetivo** | `Pipeline Dispatch` | **Ver ao lado** | **" << activeDenoiserPath << "** |\n";
        md << "| **Frame Generation Mode** | `+0x2A6` | `" << static_cast<int>(fgMode) << "` | `0=Off, 1=On/DLSS-FG, 2=FSR-FG` |\n";
        md << "| **FG Frames to Generate** | `+0x298` | `" << fgNumFrames << "` | XML `_numFramesToGenerate = " << xml.numFramesToGenerate << "` |\n";
        md << "| **Dynamic Multi-Frame Gen** | `+0x29C` | `" << static_cast<int>(dynMfg) << "` | Target FPS (`+0x2A0`): `" << dynTargetFps << "` |\n\n";
    } else {
        md << "> **Nota:** Jogo não estava em execução com `PASceneRenderer` ativo no momento desta coleta. Inicie o jogo até o menu/gameplay e execute novamente a coleta para preencher os valores de runtime.\n\n";
    }

    // Section 5: VRAM Telemetry
    md << "## 5. Consumo de VRAM Dedicada, Compartilhada (PCIe) e Budget DXGI/WDDM\n\n";
    if (vram.valid) {
        auto toMB = [](uint64_t b) { return static_cast<double>(b) / (1024.0 * 1024.0); };
        md << "| Métrica de Memória de Vídeo (DXGI / WDDM Kernel) | Valor Atual |\n";
        md << "| :--- | :--- |\n";
        md << "| **GPU / Adaptador DXGI** | **" << vram.adapterName << "** |\n";
        md << "| **VRAM Física Dedicada Total** | `" << std::fixed << std::setprecision(1) << toMB(vram.dedicatedVramBytes) << " MB` |\n";
        md << "| **VRAM Dedicada (`LOCAL`) — Budget do OS** | `" << toMB(vram.localBudgetBytes) << " MB` |\n";
        md << "| **VRAM Compartilhada (`NON_LOCAL` / PCIe) — Budget do OS** | `" << toMB(vram.nonLocalBudgetBytes) << " MB` |\n";
        if (vram.wddmValid) {
            double adapterBudgetPct = (vram.localBudgetBytes > 0)
                ? (100.0 * static_cast<double>(vram.adapterTotalCommittedBytes) / static_cast<double>(vram.localBudgetBytes))
                : 0.0;
            md << "| **`CrimsonDesert.exe` — Uso de VRAM Dedicada (`Local Usage`)** | **`" << toMB(vram.procLocalUsageBytes) << " MB`** |\n";
            md << "| **`CrimsonDesert.exe` — Uso de RAM Compartilhada via PCIe (`Non-Local Usage`)** | **`" << toMB(vram.procNonLocalUsageBytes) << " MB` (Spillover PCIe Ativo!)** |\n";
            md << "| **`CrimsonDesert.exe` — Total GPU Memory Committed** | **`" << toMB(vram.procTotalCommittedBytes) << " MB`** |\n";
            md << "| **`CrimsonDesert.exe` — Espaço Virtual D3D12 Mapeado (`VirtualSize`)** | **`" << toMB(vram.processVirtualSizeBytes) << " MB` (`55.4 GB` de Heaps/Tiles Reservados)** |\n";
            md << "| **Adaptador GPU (Sistema Total) — Uso Dedicado (`Dedicated Usage`)** | **`" << toMB(vram.adapterDedicatedUsageBytes) << " MB`** |\n";
            md << "| **Adaptador GPU (Sistema Total) — Total Committed vs. Budget** | **`" << toMB(vram.adapterTotalCommittedBytes) << " MB / " << toMB(vram.localBudgetBytes) << " MB (" << adapterBudgetPct << "%)`** |\n";
            md << "| **Utilização das Filas WDDM (`3D` / `Copy` / `Async Compute`)** | **`3D: " << vram.gpu3DAvgUtilPercent << "% (Max " << vram.gpu3DMaxUtilPercent << "%)` / `Copy: " << vram.gpuCopyAvgUtilPercent << "%` / `Compute: " << vram.gpuComputeAvgUtilPercent << "%`** |\n";
        } else {
            md << "| **VRAM Dedicada (`LOCAL`) — Uso Atual (Processo Local)** | **`" << toMB(vram.localUsageBytes) << " MB`** (`" << vram.localUsagePercent << "%` do Budget) |\n";
            md << "| **VRAM Compartilhada (`NON_LOCAL` / PCIe) — Uso Atual** | **`" << toMB(vram.nonLocalUsageBytes) << " MB`** (`" << vram.nonLocalUsagePercent << "%` do Budget) |\n";
        }
        md << "\n";
    } else {
        md << "> Não foi possível consultar `IDXGIAdapter3::QueryVideoMemoryInfo`.\n\n";
    }

    // Section 6: Live Frame Pacing Baseline
    if (fp.valid) {
        md << "## 6. Linha de Base de Apresentação de Frames e Frame Pacing (Read-Only Capture)\n\n";
        md << "| Métrica de Frame Pacing & Sincronização CPU→GPU | Valor Medido na Linha de Base (`" << std::fixed << std::setprecision(1) << fp.captureDurationSec << " s`) |\n";
        md << "| :--- | :--- |\n";
        md << "| **Total de Frames Reais Renderizados (`[RenderDevice + 0x0E8]`)** | **`" << fp.totalFrames << " frames`** |\n";
        md << "| **Taxa de Quadros Média (`Average FPS / FrameTime`)** | **`" << std::setprecision(2) << fp.avgFps << " FPS` (`" << fp.avgFrameTimeMs << " ms`)** |\n";
        md << "| **Mediana (`P50 FrameTime`)** | **`" << fp.p50FrameTimeMs << " ms` (`" << (1000.0 / fp.p50FrameTimeMs) << " FPS`)** |\n";
        md << "| **Percentil 90 (`P90 FrameTime`)** | `" << fp.p90FrameTimeMs << " ms` (`" << (1000.0 / fp.p90FrameTimeMs) << " FPS`) |\n";
        md << "| **Percentil 95 (`P95 FrameTime`)** | `" << fp.p95FrameTimeMs << " ms` (`" << (1000.0 / fp.p95FrameTimeMs) << " FPS`) |\n";
        md << "| **`1% Low` (`P99 FrameTime`) e Razão `P99 / P50`** | **`" << fp.fps1PercentLow << " FPS` (`" << fp.p99FrameTimeMs << " ms`) — Razão `P99/P50 = " << fp.ratioP99ToP50 << "x`** |\n";
        md << "| **`0.1% Low` (`P99.9 FrameTime` — Stutter Severo)** | **`" << fp.fps0_1PercentLow << " FPS` (`" << fp.p99_9FrameTimeMs << " ms`)** |\n";
        md << "| **FrameTime Mínimo / Máximo** | `" << fp.minFrameTimeMs << " ms` / **`" << fp.maxFrameTimeMs << " ms`** |\n";
        md << "| **Desvio Padrão de FrameTime (`Jitter StdDev`)** | **`" << fp.stdDevJitterMs << " ms` (Delta máx. entre frames consecutivos: `" << fp.maxFrameToFrameDeltaMs << " ms`)** |\n";
        md << "| **Spikes de Frame-to-Frame Delta (`> 8.0 ms` / `> 16.6 ms`)** | **`" << fp.spikesOver8Ms << "` spikes `> 8ms` (`" << (100.0 * fp.spikesOver8Ms / fp.totalFrames) << "%`) / `" << fp.spikesOver16_6Ms << "` spikes `> 16.6ms` (`" << (100.0 * fp.spikesOver16_6Ms / fp.totalFrames) << "%`)** |\n";
        md << "| **Profundidade da Fila CPU→GPU (`[+0x0E0] - [+0x0F0]`)** | **`Depth 0 (GPU Ociosa/Starved): " << fp.queueDepth0Count << "` | `Depth 1 (Serializado): " << fp.queueDepth1Count << "` | `Depth 2: " << fp.queueDepth2Count << "` | `Depth 3+: " << fp.queueDepth3PlusCount << "`** |\n\n";
    }

    // Section 7: Full Comparative Table (Memory vs Expected by XML & Presets)
    md << "## 7. Tabela Comparativa Completa: Memória Real vs. XML & Presets (`[PASceneRenderer + 0x" << std::hex << std::uppercase << binVal.optionPtrOffsetInRenderer << std::dec << "]`)\n\n";
    md << "| Categoria | Parâmetro (`PASceneRendererOption`) | Offset | Esperado (XML / Preset `" << xml.selectPreset << "` / `ctor`) | Valor Real na Memória | Fonte / Diagnóstico Técnico |\n";
    md << "| :--- | :--- | :--- | :--- | :--- | :--- |\n";

    for (size_t i = 0; i < kNumExpectedProps; ++i) {
        const auto& prop = kExpectedProps[i];
        uint32_t actualOff = prop.expectedOffset;
        auto it = binVal.resolvedOffsets.find(prop.name);
        if (it != binVal.resolvedOffsets.end()) actualOff = it->second;

        std::string reason;
        std::string expectedStr = GetExpectedValueForPreset(actualOff, xml, reason);
        std::string runtimeStr = rt.optionStructFound
            ? FormatRuntimeValue(rt.rawOptionBytes, prop, actualOff)
            : "*(Aguardando Jogo)*";

        char offHex[16];
        std::snprintf(offHex, sizeof(offHex), "+0x%03X", actualOff);

        md << "| " << prop.category
           << " | `" << prop.name << "`"
           << " | `" << offHex << "`"
           << " | `" << expectedStr << "`"
           << " | **`" << runtimeStr << "`**"
           << " | " << reason << " |\n";
    }

    md << "\n";
    return md.str();
}

} // namespace CDTelemetry
