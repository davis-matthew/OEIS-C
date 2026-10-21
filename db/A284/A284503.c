/*
 * A284503  Expansion of Product_{k>=0} (1 - x^(7*k+5)) in powers of x.
 * https://oeis.org/A284503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥0} (1 - x^(7k+5))
 * Range:     i8 n <= 215, i16 n <= 580, i32 n <= 1753, i64 n <= 5772,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the product by convolution in
   O(n²/14) time, suitable for n up to at least 10 000.   */
/* Uses lazy allocation: the array of coefficients is
   recomputed only when a larger n is requested.       */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *coeff = NULL;
    static size_t coeff_size = 0;

    /* Allocate or extend the coefficient array if needed.  */
    if (coeff == NULL || (size_t)n + 1 > coeff_size)
    {
        size_t new_size = (size_t)n + 1;
        seq_t *new_coeff = NULL;

        if (coeff == NULL)
            new_coeff = (seq_t *)malloc(new_size * sizeof(seq_t));
        else
            new_coeff = (seq_t *)realloc(coeff, new_size * sizeof(seq_t));

        if (!new_coeff)          /* allocation failed → safe fallback */
        {
            if (coeff) free(coeff);
            coeff = NULL;
            coeff_size = 0;
            return (seq_t)0;
        }

        /* Initialise all entries to zero.  */
        for (size_t i = 0; i < new_size; ++i)
            new_coeff[i] = 0;

        /* Base coefficient a(0) = 1.  */
        new_coeff[0] = 1;

        /* Expand the product ∏(1 - x^m) for all parts m = 5,12,19,… ≤ n. */
        for (idx_t m = 5; m <= n; m += 7)
        {
            /* Perform convolution in descending order so each part
               is used at most once (classic subset‑sum DP). */
            for (idx_t j = n; j >= m; --j)
                new_coeff[j] -= new_coeff[j - m];
        }

        coeff = new_coeff;
        coeff_size = new_size;
    }

    return coeff[n];
}
