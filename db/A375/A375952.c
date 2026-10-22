/*
 * A375952  Expansion of e.g.f. 1 / (4 - 3 * exp(x))^(5/3).
 * https://oeis.org/A375952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (1/2) * Sum_{k=0..n} A008544(k+1) * Stirling2(n,k)
 * This implementation uses a direct recurrence derived from the e.g.f.
 * (4 - 3*exp(x))^(-5/3).
 *
 * Recurrence:
 *   for n >= 0
 *     a_{n+1} = 3 * Σ_{j=0}^{n-1} C(n,j) * a_{j+1}
 *               + 5 * Σ_{j=0}^{n  } C(n,j) * a_{j}
 *
 * with a_0 = 1.
 *
 * The sums are evaluated with a running binomial coefficient to keep
 * the algorithm O(n^2).  The code is memoised so repeated calls with
 * arbitrary n values are handled efficiently.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static memoisation tables ------------------------------------------- */
    static seq_t *a_vals  = NULL;     /* a_vals[i] = a(i)                */
    static size_t allocated = 0;      /* current array capacity           */
    static idx_t  computed  = 0;      /* largest index already computed   */

    /* Initialisation ------------------------------------------------------- */
    if (a_vals == NULL) {
        allocated = 1;
        a_vals = (seq_t *)malloc(allocated * sizeof(seq_t));
        if (!a_vals) return (seq_t)0;          /* allocation failure    */
        a_vals[0] = (seq_t)1;                  /* a(0) = 1 */
    }

    /* Grow array if necessary ------------------------------------------------ */
    if ((size_t)(n + 1) > allocated) {
        size_t newSize = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(a_vals, newSize * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* allocation failure    */
        a_vals = tmp;
        allocated = newSize;
    }

    /* Compute values not yet present ---------------------------------------- */
    for (idx_t idx = computed + 1; idx <= n; ++idx) {
        /* idx is the target index a(idx).  Use recurrence with n = idx-1.   */
        idx_t np = idx - 1;                 /* the n in the recurrence      */

        seq_t sum1 = 0;                     /* Σ_{j=0}^{np-1} C(np,j)*a[j+1] */
        seq_t sum2 = 0;                     /* Σ_{j=0}^{np  } C(np,j)*a[j]   */
        seq_t binom = 1;                    /* C(np,0) = 1                   */

        for (idx_t j = 0; j <= np; ++j) {
            /* Accumulate terms of both sums                               */
            sum2 += binom * a_vals[j];
            if (j < np)                     /* only up to j = np-1 for sum1   */
                sum1 += binom * a_vals[j + 1];

            /* Update binomial coefficient for next j (if j < np)         */
            if (j < np) {
                binom = binom * (np - j);          /* easier form of C(np,j+1)      */
                binom = binom / (j + 1);
            }
        }

        /* a(idx) = 3*sum1 + 5*sum2                                         */
        a_vals[idx] = 3 * sum1 + 5 * sum2;
    }

    computed = n;
    return a_vals[n];
}
