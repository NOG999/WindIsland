#pragma once

#include "ThemeTypes.h"

namespace windisland {

class ThemeManager {
public:
    ThemeManager() = default;

    void SetPreset(ThemePreset preset) noexcept;
    void RefreshSystemColors() noexcept;

    [[nodiscard]] const GlassTheme& Current() const noexcept {
        return current_;
    }

private:
    static ThemeColor ResolveWindowsAccent() noexcept;
    static bool IsSystemDarkMode() noexcept;
    void Rebuild() noexcept;

    ThemePreset preset_ = ThemePreset::SystemGlass;
    GlassTheme current_{};
};

}  // namespace windisland
