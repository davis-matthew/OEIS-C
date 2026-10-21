/*
 * A263526  Expansion of f(x, x)^2 / (f(x^3, x^3) * f(x, x^5)) in powers of x
 *          where f(, ) is Ramanujan's general theta function.
 * https://oeis.org/A263526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of xⁿ in ∏_{k≥1} (1 - x^k)^{-e_k} with
 *            the 12‑periodic exponent e_k = [ 3, -5, 2, -2, 3, -2, 3, -2, 2,
 *            -5, 3, 0 ]
 * Range:     i8 n <= 42, i16 n <= 132, i32 n <= 462, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: for e_k = 3 the factor is (1 - x)^3, so we multiply by
   (1 - x)^(-3) = 1/(1 - x)^3, i.e. we apply 3 times the operator
   cur[i] += cur[i-1].  For e_k = -5 we multiply by (1 - x^2)^5,
   i.e. we apply 5 times the operator cur[i] -= cur[i-2].  This
   pattern is implemented below.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1; /* a(0) = 1 */

    /* Convert index to size_t for array bounds */
    size_t N = (size_t)n;

    /* Allocate array of N+1 coefficients, initialized to 0 */
    seq_t *cur = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!cur)
        return (seq_t)0; /* memory allocation failure */

    cur[0] = (seq_t)1; /* start with 1 */

    /* Periodic exponent sequence e_k (period 12) */
    const int period = 12;
    const int e[12] = {  3, -5,  2, -2,  3, -2,
                        3, -2,  2, -5,  3,  0 };

    /* Apply factors (1 - x^k)^(-e_k) for k = 1 … N */
    for (size_t k = 1; k <= N; ++k)
    {
        int ek = -e[(k - 1) % period]; /* exponent we actually need */
        if (ek == 0)
            continue;

        if (ek > 0)        /* multiply by (1 - x^k)^ek  (ek positive) */
        {
            /* Descending loop uses old values of cur[i-k] */
            for (int t = 0; t < ek; ++t)
            {
                for (long i = (long)N; i >= (long)k; --i)
                {
                    cur[i] -= cur[i - k];
                }
            }
        }
        else               /* ek < 0: divide by (1 - x^k)^{|ek|} */
        {
            int m = -ek;   /* number of ascending passes */
            for (int t = 0; t < m; ++t)
            {
                for (size_t i = k; i <= N; ++i)
                {
                    cur[i] += cur[i - k];
                }
            }
        }
    }

    seq_t result = cur[N];
    free(cur);
    return result;
}
