/*
 * A370196  Coefficient of x^n in the expansion of ( (1+x)^2 * (1+x^3) )^n.
 * https://oeis.org/A370196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊n/3⌋} C(n,k) * C(2·n, n-3k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 994 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (C(m,r) denotes binomial coefficient)                      */

#include <stddef.h>          /* for size_t if needed */
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t t1 = binomial(n, k);          /* C(n,k)   */
        seq_t t2 = binomial(2 * n, n - 3 * k); /* C(2n, n-3k) */
        sum += t1 * t2;
    }

    return sum;
}
