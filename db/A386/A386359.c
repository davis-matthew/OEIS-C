/*
 * A386359  a(n) = (1/4)*(9^n - 2*4^n + 3), n > 0; a(0) = 0.
 * https://oeis.org/A386359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/4)*(9^n - 2*4^n + 3), n > 0; a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 284 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
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
        return (seq_t)0;

    const seq_t nine = (seq_t)9;
    const seq_t four = (seq_t)4;

    seq_t pow9 = ipow_idx(nine, n);
    seq_t pow4 = ipow_idx(four, n);

    seq_t two_pow4 = mul_small(pow4, 2);   /* 2 * 4^n */
    seq_t val = pow9;                     /* 9^n */
    val -= two_pow4;                      /* 9^n - 2*4^n */
    val += (seq_t)3;                      /* + 3 */
    return val / 4;                       /* divide by 4 (exact) */
}
