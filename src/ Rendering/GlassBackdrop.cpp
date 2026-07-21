#include "GlassBackdrop.h"

#include <algorithm>

namespace windisland {
namespace {

bool SameColor(const ThemeColor& a, const ThemeColor& b) noexcept {
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

bool SameBrushTheme(const GlassTheme& a, const GlassTheme& b) noexcept {
    return SameColor(a.surface, b.surface) &&
           SameColor(a.border, b.border) &&
           SameColor(a.innerHighlight, b.innerHighlight);
}

}  // namespace

bool GlassBackdrop::Initialize(ID2D1RenderTarget* target) noexcept {
    Reset();
    target_ = target;
    return target_ != nullptr;
}

void GlassBackdrop::Reset() noexcept {
    surfaceBrush_.Reset();
    borderBrush_.Reset();
    highlightBrush_.Reset();
    target_ = nullptr;
    hasCachedTheme_ = false;
}

bool GlassBackdrop::EnsureBrushes(const GlassTheme& theme) noexcept {
    if (!target_) {
        return false;
    }

    if (hasCachedTheme_ && SameBrushTheme(cachedTheme_, theme) &&
        surfaceBrush_ && borderBrush_ && highlightBrush_) {
        return true;
    }

    surfaceBrush_.Reset();
    borderBrush_.Reset();
    highlightBrush_.Reset();

    if (FAILED(target_->CreateSolidColorBrush(
            theme.surface.ToD2D(), surfaceBrush_.GetAddressOf()))) {
        return false;
    }

    if (FAILED(target_->CreateSolidColorBrush(
            theme.border.ToD2D(), borderBrush_.GetAddressOf()))) {
        return false;
    }

    if (FAILED(target_->CreateSolidColorBrush(
            theme.innerHighlight.ToD2D(), highlightBrush_.GetAddressOf()))) {
        return false;
    }

    cachedTheme_ = theme;
    hasCachedTheme_ = true;
    return true;
}

void GlassBackdrop::Draw(
    const D2D1_ROUNDED_RECT& bounds,
    const GlassTheme& theme) noexcept {
    if (!EnsureBrushes(theme)) {
        return;
    }

    target_->FillRoundedRectangle(bounds, surfaceBrush_.Get());
    target_->DrawRoundedRectangle(
        bounds,
        borderBrush_.Get(),
        std::max(0.5f, theme.borderWidth));

    // A subtle top-edge highlight gives the surface depth without creating
    // the bright outline associated with an Apple-style pill.
    D2D1_ROUNDED_RECT highlight = bounds;
    highlight.rect.left += 1.0f;
    highlight.rect.top += 1.0f;
    highlight.rect.right -= 1.0f;
    highlight.rect.bottom = highlight.rect.top + 1.0f;

    target_->DrawRoundedRectangle(
        highlight,
        highlightBrush_.Get(),
        1.0f);
}

}  // namespace windisland
