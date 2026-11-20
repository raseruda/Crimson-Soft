# Crimson Soft

A lightweight, single-file proxy mod (**`pdh.dll` recommended** / `winmm.dll` optional) for **Crimson Desert** that removes the engine's harsh post-process sharpening filter for a softer, more natural image, and adds real-time **Depth of Field** and **Motion Blur** controls.

> **Why `pdh.dll`?** Unlike `winmm.dll`, `dxgi.dll`, or `version.dll`, Windows' Performance Data Helper (`pdh.dll`) is statically imported by `CrimsonDesert.exe` yet rarely touched by other mods, ReShade, OptiScaler, or mod loaders—allowing multiple mods to coexist cleanly.

> **Honest Scope:** This mod does **not** alter FPS or hook DirectX/DLSS pipelines. It safely toggles built-in BlackSpace Engine post-process options (`PASceneRendererOption`) in memory, works seamlessly with **DLSS Frame Generation**, and leaves zero permanent changes to game files.

---

## Features

- **`F9` — Instant Toggle:** Enables/disables `_enablePostProcessSharpen` (`0` = soft image, `1` = game default) in real time with an optional subtle audio beep.
- **`F10` — In-Game Overlay Menu (Borderless Windowed):**
  - Enable/disable Post-Process Sharpen (`[F9]`)
  - Toggle **Depth of Field**
  - Toggle **Motion Blur**
  - Adjust **Motion Blur Intensity (Camera Movement)** (`0.00x` – `2.00x` via `_masterBlurScale`)
  - Automatically tracks engine struct reallocations when toggling **Frame Generation** or loading scenes.
- **No `winmm.dll` / `dxgi.dll` Conflicts:** `pdh.dll` coexists alongside other mods and overlays.
- **Dynamic Offset Resolution:** Scans `CrimsonDesert.exe` in memory on startup so it stays compatible across game updates (tested on `v1.0.0.3732`).
- **Zero DX12 Hooks & Zero Network Access:** No DirectX/Streamline hooks, no telemetry, no internet access.

---

## Installation

1. Download **`CrimsonSoft-v1.0.1-pdh.zip`** from [Releases](../../releases) (built directly from this public repository via GitHub Actions).
2. Extract **`pdh.dll`** into your Crimson Desert `bin64` folder:
   ```text
   ...\Steam\steamapps\common\Crimson Desert\bin64\pdh.dll
   ```
   *(If you previously installed `winmm.dll` from v1.0.0, delete `winmm.dll` first so only one copy runs.)*
3. Launch the game (**Borderless Windowed** recommended so the `F10` menu displays over the game).

### Uninstall
Delete `pdh.dll` from the `bin64\` folder. Nothing else is modified.

---

## Building from Source (MSVC x64)

```bat
mkdir build
rc /nologo /fo build\pdh.res src\pdh.rc
ml64 /nologo /c /Fo build\pdh_forwards.obj src\pdh_forwards.asm
cl /nologo /O2 /EHsc /std:c++17 /W3 /LD /Isrc src\pdh_mod.cpp src\pdh_exports.cpp build\pdh_forwards.obj build\pdh.res user32.lib gdi32.lib comctl32.lib /Fe:build\pdh.dll /Fo:build\ /link /DEF:src\pdh.def
```

---

## License

MIT License — see [LICENSE](LICENSE).
