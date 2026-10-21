/*
 * A293003  Expansion of Product_{k>=1} (1 - x^(2*k))/(1 - x^(3*k)).
 * https://oeis.org/A293003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 - x^(2k)) / (1 - x^(3k))
 * Range:     i8 n <= 44, i16 n <= 128, i32 n <= 404, i64 n <= 1382,
 *            i128 n <= 4997, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code computes the product by successive polynomial convolutions. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate an array for the coefficients up to degree n. */
    seq_t *res = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (res == NULL)
        return 0;   /* allocation failed */

    /* initialise */
    if (n >= 1)
        for (idx_t i = 1; i <= n; ++i)
            res[i] = 0;
    res[0] = 1;

    /* Multiply by (1 - x^(2k)) for all k such that 2k ≤ n. */
    for (idx_t k = 1; (idx_t)(2 * k) <= n; ++k) {
        idx_t shift = (idx_t)(2 * k);
        for (idx_t i = n; i >= shift; --i)
            res[i] -= res[i - shift];
    }

    /* Multiply by 1/(1 - x^(3k)) = 1 + x^(3k) + x^(6k) + ... for all k with 3k ≤ n. */
    for (idx_t k = 1; (idx_t)(3 * k) <= n; ++k) {
        idx_t d = (idx_t)(3 * k);
        for (idx_t i = d; i <= n; ++i)
            res[i] += res[i - d];
    }

    seq_t ans = res[n];
    free(res);
    return ans;
}
