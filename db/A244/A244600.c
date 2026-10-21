/*
 * A244600  Expansion of f(-x) / f(-x^7) in powers of x where f() is a
 *          Ramanujan theta function.
 * https://oeis.org/A244600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k>0, k mod 7 ≠ 0} (1 - x^k)
 * Range:     i8 n <= 90, i16 n <= 272, i32 n <= 888, i64 n <= 3079,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the generating function
   G(x) = (∏_{k=1}^∞ (1 - x^k)) / (∏_{k=1}^∞ (1 - x^{7k})),
   i.e. the product over all positive integers not divisible by 7.
   The algorithm uses dynamic programming over the factors
   (1 - x^k) (k not ≡ 0 (mod 7)).  It runs in O(n²) time
   and O(n) space, which is well below the 2‑second limit
   for n ≤ 10000.  The function works for any
   seq_t capable of holding the values. */

#include <stdlib.h>   /* calloc, free */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* base case */

    /* allocate array of coefficients [0..n] */
    size_t len = (size_t)(n + 1);
    seq_t *coef = (seq_t *)calloc(len, sizeof(seq_t));
    if (!coef) return (seq_t)0;           /* memory allocation failure */

    coef[0] = (seq_t)1;                   /* empty product gives 1 */

    /* multiply sequentially by (1 - x^k) for all k not
       divisible by 7, updating the coefficient array in place.
       The inner loop iterates downward to avoid using
       freshly updated values within the same factor. */
    for (idx_t part = 1; part <= n; ++part) {
        if ((part % 7) == 0) continue;    /* skip multiples of 7 */

        for (idx_t k = n; ; --k) {
            coef[k] -= coef[k - part];
            if (k == part) break;         /* terminate loop when k == part */
        }
    }

    seq_t result = coef[n];
    free(coef);
    return result;
}
