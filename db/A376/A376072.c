/*
 * A376072  a(n) are half the sums of the gamma coefficients of the n-th
 *          row-generating function of triangle A375853.
 * https://oeis.org/A376072
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + n*(3^(n-1)+(-1)^n)/8 a(2) = 1, a(3) = 4, a(4) =
 *            18,
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined by the recurrence above.
   We memoize values up to the maximum n seen so far.
   pow3_state holds 3^(current_cached-1) and is updated in O(1)
   per new element. 64‑bit arithmetic for small values, unsigned
   integer types for bigger ones.  */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Return 0 for n < 2 (outside the OEIS offset; not expected) */
    if (n < 2) return 0;

    /* Static cache for memoization */
    static seq_t *cache = NULL;          /* array of a[0]..a[max_cached] */
    static idx_t max_cached = 0;         /* highest index that has a value */
    static seq_t pow3_state = 3;         /* 3^(max_cached-1),  i.e. 3^(current index -1) */

    /* Initialise cache on first use */
    if (cache == NULL) {
        max_cached = 2;
        cache = (seq_t*)malloc((max_cached + 1) * sizeof(seq_t));
        for (idx_t i = 0; i <= max_cached; ++i) cache[i] = 0;
        cache[2] = (seq_t)1;      /* a(2) = 1 */
        pow3_state = 3;           /* 3^(2-1) = 3 */
    }

    /* Extend cache up to n if necessary */
    if (n > max_cached) {
        /* Allocate more space */
        cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));

        /* Compute new terms consecutively */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            /* Update 3^(i-1) */
            pow3_state *= (seq_t)3;          /* now pow3_state = 3^(i-1) */

            /* Compute term = i * (3^(i-1) + (-1)^i) / 8  */
            seq_t term;
            if ((i & (idx_t)1) == 0) { /* even i : (-1)^i = 1 */
                term = pow3_state + (seq_t)1;
            } else {                  /* odd i  : (-1)^i = -1 */
                term = pow3_state - (seq_t)1;
            }
            term *= i;
            term /= (seq_t)8;            /* guaranteed to divide evenly */

            /* a(i) = a(i-1) + term */
            cache[i] = cache[i - 1] + term;
        }
        max_cached = n;
    }

    return cache[n];
}
