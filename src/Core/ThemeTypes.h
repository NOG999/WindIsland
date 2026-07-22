#pragma once

#include <d2d1.h>
#include <windows.h>

namespace windisland {

enum class ThemePreset {
    SystemGlass,
    OledBlack,
    ClearAcrylic,
};

struct ThemeColor {
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 1.0f;

    constexpr D2D1_COLOR_F ToD2D() const noexcept {
        return D2D1::ColorF(r, g, b, a);
    }
};

struct GlassTheme {
    ThemePreset preset = ThemePreset::SystemGlass;

    ThemeColor surface{0.035f, 0.040f, 0.050f, 0.72f};
    ThemeColor border{0.70f, 0.75f, 0.82f, 0.22f};
    ThemeColor innerHighlight{1.0f, 1.0f, 1.0f, 0.07f};
    ThemeColor primaryText{0.96f, 0.96f, 0.98f, 1.0f};
    ThemeColor secondaryText{0.68f, 0.70f, 0.75f, 1.0f};
    ThemeColor accent{0.0f, 0.47f, 0.84f, 1.0f};
    ThemeColor shadow{0.0f, 0.0f, 0.0f, 0.45f};

    float cornerRadius = 24.0f;
    float borderWidth = 1.0f;
    float blurAmount = 28.0f;
    float shadowBlur = 24.0f;
};

}  // namespace windisland
