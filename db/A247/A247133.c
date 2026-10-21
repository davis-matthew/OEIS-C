/*
 * A247133  Expansion of f(-x, -x^11) in powers of x where f() is a Ramanujan
 *          theta function.
 * https://oeis.org/A247133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. = Product_{k>0} (1 - x^(12*k)) * (1 - x^(12*k - 1)) * (1 -
 *            x^(12*k - 11))
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is coefficient of x^n in that product, a(0) = 1. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *coeff = NULL;   /* array of coefficients up to current size-1 */
    static size_t size = 0;       /* number of allocated coefficients */

    /* if we don't have enough coefficients, recompute up to n */
    if (size <= n) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(coeff, new_size * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failed */
        coeff = tmp;

        /* initialise all new entries to zero and set a(0)=1 */
        for (size_t i = 0; i < new_size; ++i) coeff[i] = 0;
        coeff[0] = 1;

        /* compute the product (1 - x^m) for all m with m % 12 == 0,1,11 */
        for (size_t m = 1; m <= n; ++m) {
            int rem = (int)(m % 12);
            if (rem == 0 || rem == 1 || rem == 11) {
                for (size_t j = n; j >= m; --j) {
                    coeff[j] -= coeff[j - m];
                }
            }
        }
        size = new_size;
    }

    return coeff[(size_t)n];
}
