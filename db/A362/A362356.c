/*
 * A362356  a(n) = 5*(n + 5)^(n-1).
 * https://oeis.org/A362356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(n+5)^(n-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
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
    if (n == 0)
        return (seq_t)1;           /* a(0) = 1 */

    seq_t base   = (seq_t)(n + 5);  /* (n+5) */
    seq_t power  = ipow_idx(base, n - 1); /* (n+5)^(n-1) */
    return mul_small(power, 5);    /* 5 * (n+5)^(n-1) */
}
