/*
 * A298249  Expansion of Product_{k>=1} (1 - x^(k*(k+1)*(2*k+1)/6)).
 * https://oeis.org/A298249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 104, i16 n <= 104, i32 n <= 104, i64 n <= 104,
 *            i128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A298249:  Expansion of the product ∏_{k≥1} (1 - x^(k(k+1)(2k+1)/6)).
 *  Each coefficient counts the difference between
 *  partitions of n into an even number of distinct square pyramidal numbers
 *  and partitions into an odd number of such numbers.
 *  Computation is by the standard distinct‑coin DP:
 *      coeff[0] = 1
 *      for each weight s:  for i = N..s  coeff[i] -= coeff[i-s]
 *
 *  Complexity: O(N * number_of_weights) ≈ O(N^(4/3)).
 *  Memory: O(N).
 */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* allocate an array of size n+1 */
    size_t sz = (size_t)(n + 1);
    seq_t *coeff = NULL;
    if (sz > 0) {
        coeff = (seq_t*)malloc(sz * sizeof(seq_t));
        if (!coeff) return (seq_t)0;           /* allocation failed */
        /* initialise */
        for (idx_t i = 0; i <= n; ++i)
            coeff[i] = (seq_t)0;
        coeff[0] = (seq_t)1;
    } else {
        /* n == 0 case: coeff[0] = 1 */
        return (seq_t)1;
    }

    /* iterate over all square‑pyramidal numbers s <= n */
    for (idx_t k = 1;; ++k) {
        idx_t s = k * (k + 1) * (2 * k + 1) / 6;      /* s <= n ? */
        if (s > n) break;

        /* multiply by (1 - x^s): subtract previous coefficients */
        for (idx_t j = n; j >= s; --j) {
            coeff[j] -= coeff[j - s];
        }
    }

    seq_t result = coeff[n];
    free(coeff);
    return result;
}
