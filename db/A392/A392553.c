/*
 * A392553  Expansion of 1 / ((1-x)^2 - x^5)^2.
 * https://oeis.org/A392553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/5⌋} (k+1)*binomial(n-3*k+3, n-5*k)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 43, u64 n <= 93, u128 n <= 196,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>               /* for size_t; no external types needed */
#include "../utils/binomial.h"    /* declares: seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 5; ++k) {
        idx_t n1 = n - 3 * k + 3;   /* n-3k+3 >= 0 for all k in the loop */
        idx_t n2 = n - 5 * k;       /* n-5k   >= 0 as well */
        seq_t b   = binomial(n1, n2);                 /* C(n-3k+3, n-5k)    */
        seq_t term = (seq_t)(k + 1) * b;              /* (k+1) * b          */
        sum += term;                                   /* accumulate          */
    }
    return sum;
}
