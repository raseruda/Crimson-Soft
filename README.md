# Crimson Soft

A lightweight, single-file `winmm.dll` proxy mod for **Crimson Desert** that removes the engine's harsh post-process sharpening filter for a softer, more natural image, and adds real-time **Depth of Field** and **Motion Blur** controls.

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
- **Dynamic Offset Resolution:** Scans `CrimsonDesert.exe` in memory on startup so it stays compatible across game updates (tested on `v1.0.0.3732`).
- **Zero DX12 Hooks & Zero Network Access:** No DirectX/Streamline hooks, no telemetry, no internet access.

---

## Installation

1. Download `winmm.dll` from [Releases](../../releases) (built directly from this public repository via GitHub Actions).
2. Copy `winmm.dll` into your Crimson Desert `bin64` folder:
   ```text
   ...\Steam\steamapps\common\Crimson Desert\bin64\winmm.dll
   ```
3. Launch the game (**Borderless Windowed** recommended so the `F10` menu displays over the game).

### Uninstall
Delete `winmm.dll` from the `bin64\` folder. Nothing else is modified.

---

## Building from Source (MSVC x64)

```bat
rc /nologo /fo build\winmm.res src\winmm.rc
ml64 /nologo /c /Fo build\winmm_forwards.obj src\winmm_forwards.asm
cl /nologo /O2 /EHsc /std:c++17 /W3 /LD /Isrc src\winmm_mod.cpp src\winmm_exports.cpp build\winmm_forwards.obj build\winmm.res user32.lib gdi32.lib comctl32.lib /Fe:build\winmm.dll /Fo:build\ /link /DEF:src\winmm.def
```

---

## License

MIT License — see [LICENSE](LICENSE).
