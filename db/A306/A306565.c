/*
 * A306565  Expansion of Product_{k>=1} (1 - x^k * (1 + x)).
 * https://oeis.org/A306565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 38, i16 n <= 133, i32 n <= 434, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expansion of Product_{k≥1} (1 - x^k * (1 + x))   */
/* The sequence a(n) is defined by the coefficient of x^n in the product ∏_{k≥1}(1 - x^k - x^{k+1}) */
/* Algorithm: iterate k = 1…n, updating a polynomial of degree n in place. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* allocate array holding coefficients for 0 … n */
    size_t sz = (size_t)n + 1;
    seq_t *arr = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!arr)
        return 0;                      /* allocation failure */

    /* initialise all coefficients to 0, constant term to 1 */
    for (size_t i = 0; i < sz; ++i)
        arr[i] = 0;
    arr[0] = 1;

    /* multiply successively by (1 - x^k - x^{k+1}) for k = 1 … n */
    for (idx_t k = 1; k <= n; ++k) {
        /* update coefficients for degrees > k, descending to keep values unchanged during the step */
        for (idx_t i = n; i > k; --i) {
            arr[i] -= arr[i - k];
            arr[i] -= arr[i - k - 1];
        }
        /* coefficient of x^k is affected by the constant term */
        arr[k] -= arr[0];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}
