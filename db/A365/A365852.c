/*
 * A365852  a(1)= 2. For n > 1, a(n) is the least number k such that k, k -
 *          a(n-1) and k + a(n-1) all have n prime divisors counted by
 *          multiplicity.
 * https://oeis.org/A365852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-2) * 3 * (27 + 4 * (-1)^n)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 33 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;

    /* 2^(n-2) */
    seq_t two_pow = ipow((seq_t)2, (seq_t)(n - 2));

    /* 3 * (27 + 4 * (-1)^n) */
    int sign = (n & 1) ? -1 : 1;
    int tmp  = 27 + 4 * sign;          /* 27 +/- 4 */
    seq_t factor = (seq_t)(3 * tmp);    /* small constant multiplication */

    return seq_mul(two_pow, factor);
}
