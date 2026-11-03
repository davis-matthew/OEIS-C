/* ipow_u128_idx: b^e.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_IPOW_U128_IDX_H
#define OEISC_UTILS_IPOW_U128_IDX_H
#include <stdint.h>

/* Helper for 128‑bit exponentiation (used only when seq_t fits in 64 bits). */
static unsigned __int128 ipow_u128_idx(unsigned __int128 base, idx_t exp)
{
    unsigned __int128 res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return res;
}

#endif
