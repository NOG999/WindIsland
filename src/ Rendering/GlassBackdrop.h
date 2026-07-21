#pragma once

#include <d2d1.h>
#include <wrl/client.h>

#include "../Core/ThemeTypes.h"

namespace windisland {

class GlassBackdrop {
public:
    bool Initialize(ID2D1RenderTarget* target) noexcept;
    void Reset() noexcept;

    void Draw(
        const D2D1_ROUNDED_RECT& bounds,
        const GlassTheme& theme) noexcept;

private:
    bool EnsureBrushes(const GlassTheme& theme) noexcept;

    ID2D1RenderTarget* target_ = nullptr;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> surfaceBrush_;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> borderBrush_;
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> highlightBrush_;

    GlassTheme cachedTheme_{};
    bool hasCachedTheme_ = false;
};

}  // namespace windisland
