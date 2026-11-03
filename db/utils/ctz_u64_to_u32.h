/* ctz_u64_to_u32: the number of trailing zero bits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_CTZ_U64_TO_U32_H
#define OEISC_UTILS_CTZ_U64_TO_U32_H
#include <stdint.h>

/* Return exponent of 2 dividing x (trailing zeros). 0→64. */
static unsigned ctz_u64_to_u32(uint64_t x)
{
    if (x == 0) return 64;
    return (unsigned)__builtin_ctzll(x);
}

#endif
