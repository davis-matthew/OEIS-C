/*
 * A258143  Row sums of A257241, Stifel's version of the arithmetical
 *          triangle.
 * https://oeis.org/A258143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{m = 1 .. ceil(n/2)} binomial(n, m)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Return 0 for n == 0 to keep the function safe even if called
       outside the intended domain (OEIS offset is 1). */
    if (n == 0) return 0;

    seq_t sum   = 0;
    seq_t binom = 1;                /* C(n,0) */
    idx_t maxm  = (n + 1) / 2;      /* ceil(n/2) */

    for (idx_t m = 1; m <= maxm; ++m)
    {
        /* Update binom to C(n, m) from C(n, m-1) */
        seq_t num = (seq_t)(n - m + 1);
        seq_t den = (seq_t)m;

        /* Reduce binom with the denominator first */
        seq_t g1 = gcd(binom, den);
        binom /= g1;
        den   /= g1;

        /* Reduce numerator with the remaining denominator */
        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2;

        /* Now denominator divides binom * num exactly */
        binom *= num;
        binom /= den;

        sum += binom;
    }

    return sum;
}
