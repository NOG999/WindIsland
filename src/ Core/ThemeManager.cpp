#include "ThemeManager.h"

#include <dwmapi.h>

namespace windisland {
namespace {

ThemeColor FromColorRef(COLORREF color, float alpha = 1.0f) noexcept {
    return {
        GetRValue(color) / 255.0f,
        GetGValue(color) / 255.0f,
        GetBValue(color) / 255.0f,
        alpha,
    };
}

}  // namespace

void ThemeManager::SetPreset(ThemePreset preset) noexcept {
    preset_ = preset;
    Rebuild();
}

void ThemeManager::RefreshSystemColors() noexcept {
    Rebuild();
}

ThemeColor ThemeManager::ResolveWindowsAccent() noexcept {
    DWORD color = 0;
    BOOL opaque = FALSE;

    if (SUCCEEDED(DwmGetColorizationColor(&color, &opaque))) {
        const COLORREF rgb = RGB(
            (color >> 16) & 0xFF,
            (color >> 8) & 0xFF,
            color & 0xFF);
        return FromColorRef(rgb);
    }

    return {0.0f, 0.47f, 0.84f, 1.0f};
}

bool ThemeManager::IsSystemDarkMode() noexcept {
    DWORD value = 0;
    DWORD size = sizeof(value);

    const LSTATUS status = RegGetValueW(
        HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
        L"AppsUseLightTheme",
        RRF_RT_REG_DWORD,
        nullptr,
        &value,
        &size);

    return status == ERROR_SUCCESS ? value == 0 : true;
}

void ThemeManager::Rebuild() noexcept {
    GlassTheme next{};
    next.preset = preset_;
    next.accent = ResolveWindowsAccent();

    switch (preset_) {
        case ThemePreset::SystemGlass:
            if (IsSystemDarkMode()) {
                next.surface = {0.035f, 0.040f, 0.050f, 0.72f};
                next.border = {0.70f, 0.75f, 0.82f, 0.22f};
                next.primaryText = {0.96f, 0.96f, 0.98f, 1.0f};
                next.secondaryText = {0.68f, 0.70f, 0.75f, 1.0f};
            } else {
                next.surface = {0.93f, 0.94f, 0.96f, 0.72f};
                next.border = {0.15f, 0.17f, 0.20f, 0.18f};
                next.primaryText = {0.08f, 0.09f, 0.11f, 1.0f};
                next.secondaryText = {0.30f, 0.32f, 0.36f, 1.0f};
            }
            break;

        case ThemePreset::OledBlack:
            next.surface = {0.0f, 0.0f, 0.0f, 0.96f};
            next.border = {1.0f, 1.0f, 1.0f, 0.14f};
            next.innerHighlight = {1.0f, 1.0f, 1.0f, 0.04f};
            next.blurAmount = 0.0f;
            break;

        case ThemePreset::ClearAcrylic:
            next.surface = {0.055f, 0.060f, 0.075f, 0.48f};
            next.border = {0.80f, 0.84f, 0.90f, 0.28f};
            next.innerHighlight = {1.0f, 1.0f, 1.0f, 0.10f};
            next.blurAmount = 38.0f;
            break;
    }

    current_ = next;
}

}  // namespace windisland
