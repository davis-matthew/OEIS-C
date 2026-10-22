/*
 * A301588  Expansion of Product_{k>=1} (1 + x^k)^(mod(k,3)).
 * https://oeis.org/A301588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in Π_{k≥1} (1 + x^k)^(k mod 3)
 *            (mod 0→0, 1→1, 2→2)
 * Range:     u8 n <= 27, u16 n <= 75, u32 n <= 233, u64 n <= 784,
 *            u128 n <= 2807, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence starts with a(0)=1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *coeff = NULL;
    static idx_t  maxN  = 0;

    /* Allocate or recompute coefficients up to n if needed */
    if (coeff == NULL || n > maxN || maxN == 0) {
        /* Ensure the array is large enough */
        size_t cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(coeff, cap * sizeof(seq_t));
        if (!tmp) abort();
        coeff = tmp;

        /* Initialise all entries to zero; coeff[0] = 1 */
        for (idx_t i = 0; i <= n; ++i) coeff[i] = 0;
        coeff[0] = 1;

        /* Compute the product up to n */
        for (idx_t k = 1; k <= n; ++k) {
            idx_t e = k % 3;
            if (e == 0) continue;            /* (1 + x^k)^0 = 1 */
            if (e == 1) {
                /* Multiply by (1 + x^k) once */
                for (idx_t t = n; ; --t) {
                    if (t < k) break;
                    coeff[t] += coeff[t - k];
                }
            } else { /* e == 2, i.e. (1 + x^k)^2 = (1 + x^k)*(1 + x^k) */
                for (int pass = 0; pass < 2; ++pass) {
                    for (idx_t t = n; ; --t) {
                        if (t < k) break;
                        coeff[t] += coeff[t - k];
                    }
                }
            }
        }
        maxN = n;
    }

    return coeff[n];
}
