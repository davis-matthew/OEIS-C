/*
 * A377399  Expansion of e.g.f. (2 - exp(x))^4.
 * https://oeis.org/A377399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1 a(n) = 4^n - 8*3^n + 3*2^(n+3) - 32 (n > 0)
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 15, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 906 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

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
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t pow4 = ipow_i32((seq_t)4, n);
    seq_t pow3 = ipow_i32((seq_t)3, n);
    seq_t pow2 = ipow_i32((seq_t)2, n + 3);

    seq_t term1 = pow4;
    seq_t term2 = mul_small(pow3, 8);
    term2 = -term2;
    seq_t term3 = mul_small(pow2, 3);
    seq_t term4 = (seq_t)-32;

    seq_t result = term1 + term2 + term3 + term4;
    return result;
}
