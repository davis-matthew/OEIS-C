/*
 * A284316  Expansion of Product_{k>=0} (1 - x^(4*k+3)) in powers of x.
 * https://oeis.org/A284316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥0}(1 - x^{4k+3})
 * Range:     i8 n <= 123, i16 n <= 332, i32 n <= 987, i64 n <= 3233,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming (product of factors) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Allocation and deallocation macros that work in both C and C++ */
#ifdef __cplusplus
#   define ALLOC_SEQ_T(count)  new seq_t[(count)]
#   define FREE_SEQ_T(ptr)     delete [] (ptr)
#else
#   define ALLOC_SEQ_T(count)  ((seq_t*)malloc((size_t)(count) * sizeof(seq_t)))
#   define FREE_SEQ_T(ptr)     free((ptr))
#endif

seq_t a(idx_t n) {
    /* If n == 0, the product theorem gives a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate array for coefficients up to n */
    size_t sz = (size_t)(n + 1);
    seq_t *coeff = ALLOC_SEQ_T(sz);
    if (!coeff) {
        /* Allocation failed – return 0 as a safe fallback */
        return (seq_t)0;
    }

    /* Initialise: a0 = 1, others = 0 */
    coeff[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        coeff[i] = (seq_t)0;
    }

    /* Iterate over each factor (1 - x^{4k+3}) */
    for (idx_t e = 3; e <= n; e += 4) {
        /* Update coefficients in reverse to avoid reuse of updated values */
        for (idx_t i = n; i >= e; --i) {
            coeff[i] -= coeff[i - e];
        }
    }

    seq_t result = coeff[n];
    FREE_SEQ_T(coeff);
    return result;
}
