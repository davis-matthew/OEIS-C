/*
 * A257402  Expansion of chi(x) * psi(-x^3) * psi(x^12) in powers of x where
 *          psi(), chi() are Ramanujan theta functions.
 * https://oeis.org/A257402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in q^(-11/6) * eta(q^2)^2 * eta(q^3) *
 *            eta(q^24)^2 / (eta(q) * eta(q^4) * eta(q^6))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The coefficients are computed from the product expansion */

/* The function is written for C11 and C++17. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for coefficients up to n.  The array is zeroed. */
    seq_t *coeff = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!coeff) return (seq_t)0;      /* out of memory – unlikely for n≤10000 */
    coeff[0] = (seq_t)1;              /* G(0) = 1 */

    /* Iterate over each factor q^m from 1 to n.  For each m compute the
       exponent a_m of (1 - q^m) in the overall product.  Then
       modify the coefficient array by the corresponding multiplication. */
    for (idx_t m = 1; m <= n; ++m)
    {
        /* Compute the exponent a_m = (2*I(m even) + I(m%3==0) + 2*I(m%24==0))
           - 1 - I(m%4==0) - I(m%6==0)   */
        int a = 0;
        if (m % 2 == 0) a += 2;
        if (m % 3 == 0) a += 1;
        if (m % 24 == 0) a += 2;
        a -= 1;
        if (m % 4 == 0) a -= 1;
        if (m % 6 == 0) a -= 1;

        if (a == 0) continue;

        if (a > 0)       /* multiply by (1 - q^m)^a  (a times 'subtract') */
        {
            for (int t = 0; t < a; ++t)
            {
                for (idx_t i = n; i >= m; --i)
                {
                    coeff[i] -= coeff[i - m];
                }
            }
        }
        else            /* a < 0: multiply by (1 - q^m)^|a|  (|a| times 'add') */
        {
            int b = -a;
            for (int t = 0; t < b; ++t)
            {
                for (idx_t i = m; i <= n; ++i)
                {
                    coeff[i] += coeff[i - m];
                }
            }
        }
    }

    seq_t ans = coeff[n];
    free(coeff);
    return ans;
}
