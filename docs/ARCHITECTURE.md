# WindIsland Architecture

## Current upstream design

The upstream Windhawk mod is a dedicated tool mod running inside `windhawk.exe`.
It owns a layered top-level overlay window, a Direct2D renderer, shared snapshots,
and several polling/listener threads.

### Existing runtime flow

```text
Media / Notifications / Weather / Audio / Clipboard / Shell events
                              |
                              v
                         SharedState
                              |
                              v
                     Activity selection
                              |
                              v
                     Direct2D Renderer
                              |
                              v
                    Layered overlay window
```

### Existing worker threads

- Render thread: creates the overlay window and drives animation/rendering.
- Media thread: reads Global System Media Transport Controls.
- Notification thread: reads Windows notification listener data where supported.
- Weather thread: fetches weather data.
- Audio thread: captures audio information and updates visualizer/system state.
- Keyboard thread: installs a low-level keyboard hook.
- Window procedure: handles clipboard, shell, mouse, menu, and window messages.

## WindIsland migration strategy

Windhawk distributes mods as a single source file. WindIsland will still keep
modular source files in Git, then generate one distributable `.wh.cpp` file.

```text
src/**/*.h + src/**/*.cpp
          |
          v
 tools/amalgamate.py
          |
          v
 dist/wind-island.wh.cpp
```

This keeps development maintainable without breaking Windhawk distribution.

## Core layers

### Core

- `ThemeTypes`: immutable visual tokens.
- `ThemeManager`: resolves preset, Windows mode, and accent colour.
- `IslandState`: normalized current UI state.
- `Activity`: one renderable item with priority and lifetime.
- `ActivityQueue`: selects primary and secondary activities.
- `AnimationEngine`: spring values for size, opacity, and position.

### Managers

Managers collect data and publish normalized activities.

- CallManager
- MediaManager
- NotificationManager
- SystemManager
- WeatherManager

### Services

Services contain app-specific detection and integration.

- PhoneLinkService
- TeamsService
- ZoomService
- DiscordService
- WhatsAppService

### Rendering

- GlassBackdrop
- IslandRenderer
- TextRenderer
- IconRenderer
- Widget renderers

## Priority model

| Activity | Priority |
|---|---:|
| Incoming call | 100 |
| Active call | 95 |
| Screen recording/sharing | 90 |
| Privacy indicator | 88 |
| Critical battery | 85 |
| Notification | 70 |
| Clipboard | 60 |
| Volume | 55 |
| Media | 50 |
| Weather | 20 |
| Idle | 5 |

## First implementation boundary

Sprint 1 intentionally does not replace upstream media, weather, or notification
logic. It introduces theme tokens and a glass backdrop that can be integrated
into the current renderer with minimal risk.
