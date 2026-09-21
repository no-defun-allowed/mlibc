#pragma once

#include <mlibc/sysdep-signatures.hpp>

namespace mlibc {

struct FioxaSysdepTags :
    LibcLog, LibcPanic,
    AnonAllocate, AnonFree,
    ClockGet,
    Close,
    Exit,
    FutexWait, FutexWake,
    Isatty, Open, Read, Seek,
    Sleep,
    TcbSet,
    VmMap, VmUnmap,
    Write
{};

template<typename Tag>
using Sysdeps = SysdepOf<FioxaSysdepTags, Tag>;

struct SysdepTraits {
	static constexpr bool usesRtNetlink = false;
};

} // namespace mlibc
