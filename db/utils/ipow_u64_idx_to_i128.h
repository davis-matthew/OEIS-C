/* ipow_u64_idx_to_i128: b^e.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IPOW_U64_IDX_TO_I128_H
#define OEISC_UTILS_IPOW_U64_IDX_TO_I128_H
#include <stdint.h>

/* Power for small seq_t (≤64 bits). Uses signed __int128 to avoid overflow. */
static __int128 ipow_u64_idx_to_i128(uint64_t base, idx_t exp)
{
    __int128 result = 1;
    __int128 b = base;
    while (exp)
    {
        if (exp & 1U)
            result *= b;
        exp >>= 1U;
        if (exp)
            b *= b;
    }
    return result;
}

#endif
