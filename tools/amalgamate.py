#!/usr/bin/env python3
"""Build a single Windhawk-compatible source file from modular sources."""

from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
ENTRY = ROOT / "src" / "wind-island.wh.cpp"
OUTPUT = ROOT / "dist" / "wind-island.wh.cpp"

LOCAL_INCLUDE = re.compile(r'^\s*#include\s+"([^"]+)"\s*$')


def expand(path: Path, seen: set[Path]) -> str:
    path = path.resolve()
    if path in seen:
        return ""

    seen.add(path)
    output: list[str] = []

    for line in path.read_text(encoding="utf-8").splitlines():
        match = LOCAL_INCLUDE.match(line)
        if match:
            include = (path.parent / match.group(1)).resolve()
            if include.is_file() and ROOT in include.parents:
                output.append(f"// BEGIN {include.relative_to(ROOT)}")
                output.append(expand(include, seen))
                output.append(f"// END {include.relative_to(ROOT)}")
                continue
        output.append(line)

    return "\n".join(output)


def main() -> None:
    if not ENTRY.exists():
        raise SystemExit(f"Missing entry file: {ENTRY}")

    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    OUTPUT.write_text(expand(ENTRY, set()) + "\n", encoding="utf-8")
    print(f"Wrote {OUTPUT.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
