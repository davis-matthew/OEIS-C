/*
 * A331630  Numbers M such that 23 * M = 1M1, where 1M1 denotes the
 *          concatenation of 1, M and 1.
 * https://oeis.org/A331630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^(6*n-3)+1)/13, n ≥ 1
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n = 0 return 0. */
    if (n == 0)
        return 0;

    /* Compute the exponent 6*n-3.  Since n is uint64_t, this does not
       overflow for the values used in the tests. */
    idx_t exp = 6 * n - 3;

    /* 10^exp as an arbitrary‑precision (or fixed‑size) integer. */
    seq_t p = pow10(exp);

    /* a(n) = (p + 1) / 13.  The division is exact. */
    return (p + (seq_t)1) / (seq_t)13;
}
