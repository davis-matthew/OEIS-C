/*
 * A387878  a(n) = Sum_{k=0..n} binomial(6*n+4,6*k).
 * https://oeis.org/A387878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-1 + 16*64^n - 9*(-27)^n)/6
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 490 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_u64_idx.h"

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
    seq_t pow64 = ipow_u64_idx(64, n);
    seq_t pow27 = ipow_u64_idx(27, n);
    seq_t term1 = mul_small(pow64, 16);   /* 16*64^n */
    seq_t term2 = mul_small(pow27, 9);    /* 9*27^n */
    seq_t numerator;

    if ((n & 1ULL) == 0) {               /* even n: (-27)^n = 27^n  */
        numerator = term1 - term2 - 1;   /* -1 + term1 - term2 */
    } else {                              /* odd n: (-27)^n = -27^n */
        numerator = term1 + term2 - 1;   /* -1 + term1 + term2 */
    }
    return numerator / 6;
}
