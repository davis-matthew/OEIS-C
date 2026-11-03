/*
 * A352161  Numbers m such that the smallest digit in the decimal expansion of
 *          1/m is k = 8, ignoring leading and trailing 0's.
 * https://oeis.org/A352161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = 125*10^{(n-1)/2} for odd n, 1125*10^{(n-2)/2} for even n
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* Compute the nth term of OEIS A352161 */
seq_t a(idx_t n)
{
    /* Sequence offset is 1; for n < 1 behaviour is irrelevant */
    if (n == 0)
        return (seq_t)0;                 /* dummy return */

    if (n & 1U) {           /* odd n */
        idx_t e = (n - 1) / 2; /* exponent of 10 */
        seq_t p = ipow_u64_idx(10U, e);
        return mul_small(p, 125U);
    } else {                /* even n */
        idx_t e = (n - 2) / 2;
        seq_t p = ipow_u64_idx(10U, e);
        return mul_small(p, 1125U);
    }
}
