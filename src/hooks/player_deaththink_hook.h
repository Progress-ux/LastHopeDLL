#pragma once

#include <cstdint>

/*
 * Current function prologue:
 *
 *   1415f0: e8 17 2d f9 ff
 *   1415f5: 05 ff 79 19 00
 *
 * The first instruction calls __x86.get_pc_thunk.ax,
 * and the second instruction calculates the GOT address.
 *
 * We replace exactly these 10 bytes.
 */
constexpr std::size_t PATCH_SIZE = 10;

struct State
{
    uintptr_t cs_base = 0;
    uintptr_t target = 0;

    void* trampoline = nullptr;
    std::size_t trampoline_size = 0;

    unsigned char original[PATCH_SIZE] = {};

    bool installed = false;
};

extern State g_state;

namespace PlayerDeathThinkHook
{
    bool Install();
    void Uninstall();

    bool IsInstalled();
}
