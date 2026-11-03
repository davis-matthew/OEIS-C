/*
 * A324269  a(n) = 3*11^(2*n).
 * https://oeis.org/A324269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*121^n
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 474 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* Compute 121^n in arbitrary‑precision */
    seq_t power = ipow_idx((seq_t)121, n);
    /* Multiply the power by 3 */
    return mul_small(power, 3);
}
