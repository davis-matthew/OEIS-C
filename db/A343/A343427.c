/*
 * A343427  G.f.: 1 + 1^1*x/(1 + 2^2*x/(1 + 3^3*x/(1 + 4^4*x/(1 + 5^5*x/(1 +
 *          ...))))).
 * https://oeis.org/A343427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 5, i64 n <= 7, i128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A343427
   G.f.: 1 + 1^1*x/(1 + 2^2*x/(1 + 3^3*x/(1 + 4^4*x/(1 + 5^5*x/(1 + ...)))))
   The coefficients a(n) are obtained by truncating the continued fraction to n
   terms and expanding as a power series.  This implementation computes
   a(n) on demand with memoisation and works for any seq_t and idx_t
   supplied by the build.  No external libraries are required. */

#include <stdlib.h>

/* Memoised cache of computed values */
static seq_t *cache      = NULL;
static idx_t  cache_max  = 0;

/* Compute a(k) for k <= max_n.  Returns pointer to cache.  */
static void build_cache(idx_t max_n)
{
    /* Allocate arrays for the three polynomials used in the recurrence */
    seq_t *a_kp1 = (seq_t*)calloc((size_t)max_n + 1, sizeof(seq_t)); /* A_{k+1} */
    seq_t *a_kp2 = (seq_t*)calloc((size_t)max_n + 1, sizeof(seq_t)); /* A_{k+2} */
    seq_t *a_cur = (seq_t*)calloc((size_t)max_n + 1, sizeof(seq_t)); /* new A_k */

    if (!a_kp1 || !a_kp2 || !a_cur) {
        /* Memory allocation failed – in the OEIS test harness this should
           never happen.  Abort if it does. */
        exit(1);
    }

    /* Tail of the continued fraction is 1, so initialise both polynomials to 1 */
    a_kp1[0] = 1;
    a_kp2[0] = 1;

    /* Build polynomials A_1 and A_2 by descending from k = max_n down to 1 */
    for (idx_t k = max_n; k >= 1; --k) {
        /* Compute a_k = k^k  */
        seq_t ak = 1;
        for (idx_t j = 0; j < k; ++j)
            ak *= (seq_t)k;

        /* Maximum degree currently produced */
        size_t deg = (size_t)max_n - (size_t)k + 1;

        /* A_k(x) = A_{k+1}(x) + a_k * x * A_{k+2}(x) */
        for (size_t i = 0; i <= deg; ++i) {
            seq_t val = a_kp1[i];
            if (i >= 1)
                val += ak * a_kp2[i - 1];
            a_cur[i] = val;
        }

        /* Rotate the three arrays for the next iteration */
        seq_t *tmp = a_kp2;
        a_kp2   = a_kp1;
        a_kp1   = a_cur;
        a_cur   = tmp;
    }

    /* Now a_kp1 holds A_1 and a_kp2 holds A_2.  Compute the power‑series
       coefficients a[0..max_n] from A_1 / A_2.  */
    seq_t *new_cache = (seq_t*)calloc((size_t)max_n + 1, sizeof(seq_t));
    if (!new_cache) { free(a_kp1); free(a_kp2); exit(1); }

    new_cache[0] = 1;  /* a(0) = 1 */

    for (size_t n = 1; n <= (size_t)max_n; ++n) {
        seq_t sum = 0;
        for (size_t k = 1; k <= n; ++k) {
            sum += a_kp2[k] * new_cache[n - k];
        }
        new_cache[n] = a_kp1[n] - sum;   /* because (A * f) = B */
    }

    /* Replace old cache */
    free(cache);
    cache = new_cache;
    cache_max = max_n;

    /* Clean up polynomial arrays */
    free(a_kp1);
    free(a_kp2);
    free(a_cur);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    if ((idx_t)cache_max < n)
        build_cache(n);

    return cache[n];
}
