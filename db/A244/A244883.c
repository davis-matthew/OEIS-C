/*
 * A244883  Expansion of (1+6*x+16*x^2+8*x^3+x^4)/(1-x)^8.
 * https://oeis.org/A244883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)*(n+2)*(n+3)*(n*(n+4)*(n*(16*n+57)+137)+420))/2520
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 47, u64 n <= 1162,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent formula used is a(n) = a(0) + Σ_{k=1}^n Σ_{i=0}^4 p_i * C(k+7-i,6) */
/* where p = {1,6,16,8,1} and C(m,6) is the binomial coefficient. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Coefficients of the numerator polynomial */
    static const seq_t coef[5] = {1, 6, 16, 8, 1};

    /* a(0) = 1 */
    if (n == 0) return 1;

    seq_t total = 1;  /* running sum of a(k) */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t delta = 0;          /* a(k) - a(k-1) */

        for (int i = 0; i < 5; ++i)
        {
            /* Compute binomial coefficient C(k+7-i, 6) */
            seq_t bi = 1;
            unsigned long long top = (unsigned long long)(k + 6 - i);
            for (int m = 0; m < 6; ++m)
            {
                /* bi = bi * (top - m) / (m + 1) */
                bi = bi * (top - m);
                bi = bi / (m + 1);
            }

            delta += coef[i] * bi;
        }

        total += delta;
    }

    return total;
}
