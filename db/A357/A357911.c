/*
 * A357911  Expansion of Product_{k>=0} (1 - x^(11*k+1)) in powers of x.
 * https://oeis.org/A357911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   not stated
 * Range:     i8 n <= 311, i16 n <= 864, i32 n <= 2673, i64 n <= 8909,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A357911: Expansion of ∏_{k≥0} (1 – x^(11*k+1)) in powers of x.
 *  a(0) = 1; for n > 0, a(n) = coefficient of x^n in the product.
 *  The recurrence given in OEIS is not used; we compute directly by
 *  multiplying the factors (1 - x^e) for e = 1, 12, 23, … ≤ n.
 *
 *  A static cache is kept, so repeated calls are efficient.  When a
 *  larger n is requested, the entire product up to that n is recomputed
 *  from scratch – this keeps the algorithm simple and guarantees
 *  correctness for any sequence values that fit in seq_t.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of coefficients up to the largest n ever requested */
    static seq_t *coeff   = NULL;   /* array of coefficients 0 .. max_cached */
    static size_t coeff_cap = 0;    /* allocated capacity of coeff */
    static idx_t  max_cached = 0;   /* highest n that has been computed */

    /* Handle n==0 quickly */
    if (n == 0) return 1;

    /* If we have already computed up to at least n, return cached value */
    if (coeff && n <= max_cached) return coeff[n];

    /* Not yet computed up to n – recompute from scratch up to n */
    size_t new_cap = (size_t)n + 1;

    /* Allocate or reallocate the coefficient array */
    if (coeff_cap < new_cap) {
        seq_t *tmp = (coeff_cap == 0) ? (seq_t *)malloc(new_cap * sizeof(seq_t))
                                      : (seq_t *)realloc(coeff, new_cap * sizeof(seq_t));
        if (!tmp) {               /* allocation failed – return 0 */
            if (coeff) free(coeff);
            coeff = NULL;
            coeff_cap = 0;
            max_cached = 0;
            return 0;
        }
        coeff = tmp;
        /* initialise new entries to zero – coeff[0] will be set below */
        for (size_t i = coeff_cap; i < new_cap; ++i) coeff[i] = 0;
        coeff_cap = new_cap;
    }

    /* Reinitialise coefficients for the new full computation */
    coeff[0] = 1;
    for (idx_t i = 1; i <= n; ++i) coeff[i] = 0;

    /* Apply each factor (1 - x^e) for e = 1, 12, 23, ... ≤ n */
    for (idx_t e = 1; e <= n; e += 11) {
        for (idx_t i = n; i >= e; --i) {
            coeff[i] = coeff[i] - coeff[i - e];
        }
    }

    /* Record that we have computed up to n */
    max_cached = n;

    return coeff[n];
}
