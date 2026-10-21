/*
 * A275206  Expansion of (A(x)^2 - A(x^2))/2 where A(x) = A000108(x) - 1.
 * https://oeis.org/A275206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n < 3; a(n) = (C(2*n, n-2) - (n even ? C(n, n/2-1)
 *            : 0)) / n for n >= 3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the binomial() helper from "utils/binomial.h" */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Base cases: a(0) = a(1) = a(2) = 0 */
    if (n < 3)
        return 0;

    idx_t two_n = n + n;           /* 2 * n */
    idx_t k1    = n - 2;           /* n - 2  (k >= 0 for n >= 2) */
    seq_t num   = binomial(two_n, k1);

    if ((n & 1) == 0) {            /* n even: subtract second binomial term */
        idx_t k2 = (n >> 1) - 1;   /* n/2 - 1 */
        num -= binomial(n, k2);
    }

    return num / n;                /* exact division, result is integer */
}
