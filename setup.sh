#!/usr/bin/env bash
# ──────────────────────────────────────────────────────────────
# Quick setup script for Intersection Traffic Simulation
# Installs dependencies, configures, builds, and runs.
# Tested on Ubuntu/Debian. For other distros, install Qt6 and
# CMake manually, then run the build steps below.
# ──────────────────────────────────────────────────────────────
set -euo pipefail

GREEN='\033[0;32m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

info()  { echo -e "${BLUE}[INFO]${NC}  $1"; }
ok()    { echo -e "${GREEN}[OK]${NC}    $1"; }
warn()  { echo -e "${YELLOW}[WARN]${NC}  $1"; }

# ── 1. Check / install dependencies ──────────────────────────

info "Checking dependencies..."

if ! command -v cmake &>/dev/null; then
    info "Installing cmake..."
    sudo apt-get update -qq
    sudo apt-get install -y -qq cmake
    ok "cmake installed"
else
    ok "cmake found ($(cmake --version | head -1))"
fi

if ! dpkg -s qt6-base-dev &>/dev/null 2>&1; then
    info "Installing Qt6 development packages..."
    sudo apt-get update -qq
    sudo apt-get install -y -qq qt6-base-dev
    ok "Qt6 installed"
else
    ok "Qt6 found"
fi

# ── 2. Configure & Build ─────────────────────────────────────

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

info "Configuring build..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
cmake .. -DCMAKE_BUILD_TYPE=Release

info "Building..."
cmake --build . --parallel "$(nproc)"
ok "Build successful!"

# ── 3. Run ────────────────────────────────────────────────────

echo ""
echo -e "${GREEN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo -e "${GREEN}  Build complete! Launching simulation...${NC}"
echo -e "${GREEN}━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━${NC}"
echo ""

cd "$BUILD_DIR"
./Intersection-Traffic-Simulation
