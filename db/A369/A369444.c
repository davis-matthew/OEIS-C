/*
 * A369444  Expansion of (1/x) * Series_Reversion( x / ((1+x) * (1+x^4)) ).
 * https://oeis.org/A369444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/4)} binomial(n+1,k) *
 *            binomial(n+1,n-4*k)
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 25, u64 n <= 46, u128 n <= 87,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Binomial coefficients are returned as seq_t.  To avoid overflow when
       the intermediate product a*b may exceed the bit‑width of the
       particular seq_t type, the product is reduced by the greatest
       common divisors with the divisor (n+1) before the final division.
       This keeps the intermediate values within the range of seq_t
       for any n whose final result fits in seq_t. */
    idx_t upper = n / 4;
    idx_t np1   = n + 1;                      /* (n+1) used many times */

    seq_t sum = 0;
    for (idx_t k = 0; k <= upper; ++k) {
        /* Compute the two binomial terms */
        seq_t c1 = binomial(np1, k);                     /* binomial(n+1,k)          */
        seq_t c2 = binomial(np1, np1 - 1 - 4 * k);       /* binomial(n+1,n-4*k)      */

        /* Reduce c1 and c2 by the divisor (n+1), to keep intermediates
           from overflowing seq_t.  The division is exact after the
           reductions because (c1*c2)/(n+1) is an integer. */
        seq_t g1 = gcd(c1, (seq_t)np1);
        c1 = c1 / g1;
        seq_t rem = (seq_t)np1 / g1;                      /* remaining part of the divisor */

        seq_t g2 = gcd(c2, rem);
        c2 = c2 / g2;
        rem = rem / g2;

        /* Now rem divides c1*c2 exactly */
        seq_t prod = c1 * c2;      /* product fits into seq_t because of the reductions */
        seq_t term = prod / rem;   /* exact division by the remaining part of (n+1) */

        sum = sum + term;
    }
    return sum;
}
