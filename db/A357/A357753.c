/*
 * A357753  a(n) is the least square with n binary digits.
 * https://oeis.org/A357753
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A017912(n-1)^2, where A017912(k) = ceil(sqrt(2^k))
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 35 (2026-11-01)
 * Helpers:   ipow_i32, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_i32.h"
#include "../utils/pow2.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;               /* behavior below offset is irrelevant */

    seq_t power = pow2(n - 1);   /* 2^(n-1) */
    seq_t m     = floor_sqrt(power);
    seq_t m2    = seq_mul(m, m);

    if (m2 < power)              /* ceil(sqrt) */
        m = m + 1;

    return seq_mul(m, m);        /* square of the ceil sqrt */
}
