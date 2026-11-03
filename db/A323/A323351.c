/*
 * A323351  Number of ways to fill a (not necessarily square) matrix with n
 *          zeros and ones.
 * https://oeis.org/A323351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * A000005(n) for n > 0, a(0) = 1
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 32 (2026-11-01)
 * Helpers:   ipow_idx, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow_idx.h"
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* 2^n using the generic power routine */
    seq_t pow2n = ipow_idx((seq_t)2, n);

    /* number of divisors of n (A000005) */
    uint64_t d = num_divisors((uint64_t)n);

    return pow2n * (seq_t)d;
}
