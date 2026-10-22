/*
 * A302827  a(n) = (n!)^2 * Sum_{k=1..n-1} 1/(k*(n-k))^2.
 * https://oeis.org/A302827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^2 * Sum_{k=1..n-1} 1/(k*(n-k))^2 Recurrence (for n
 *            >= 4): n*(2*n - 3)*a(n) = (n-1)*(6*n^3 - 25*n^2 + 33*n -
 *            12)*a(n-1) - (n-2)^3*(6*n^3 - 29*n^2 + 42*n - 15)*a(n-2) +
 *            (n-3)^4*(n-2)^3*(2*n - 1)*a(n-3) Initial values: a(0)=0, a(1)=0,
 *            a(2)=4, a(3)=18
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 13, u128 n <= 20,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;

    /* Helper to ensure cache has at least up_to indices (0..up_to inclusive) */
    if (cache == NULL) {
        size_t init_cap = 4; /* for indices 0..3 */
        cache = alloc_seq_array(init_cap);
        cached_n = 0;
    }
    if ((idx_t)cached_n < n) {
        /* Grow cache to accommodate index n */
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = alloc_seq_array(new_cap);
        for (idx_t i = 0; i <= cached_n; ++i)
            new_cache[i] = cache[i];
        free_seq_array(cache);
        cache = new_cache;
        cached_n = n;
    }

    /* Return cached value if available */
    if (n <= cached_n && cache[n] != 0) {
        return cache[n];
    }

    /* Compute missing entries up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (i <= cached_n && cache[i] != 0)
            continue;  /* already computed */

        if (i == 0) {
            cache[0] = 0;
        } else if (i == 1) {
            cache[1] = 0;
        } else if (i == 2) {
            cache[2] = (seq_t)4;
        } else if (i == 3) {
            cache[3] = (seq_t)18;
        } else if (i <= 13) {
            /* For small n (<=13) use explicit sum to avoid overflow
               on 64‑bit builds. */
            seq_t fact = 1;
            for (idx_t j = 2; j <= i; ++j)
                fact = fact * (seq_t)j;

            seq_t sum = 0;
            for (idx_t k = 1; k < i; ++k) {
                idx_t den = k * (i - k);
                seq_t denom = (seq_t)den;
                seq_t term = fact / denom;
                term = term * term;
                sum = sum + term;
            }
            cache[i] = sum;
        } else {
            /* Recurrence for i >= 14 (valid for arbitrary‑precision seq_t) */
            seq_t sn = (seq_t)i;
            seq_t sn2 = sn * sn;
            seq_t sn3 = sn2 * sn;
            seq_t sn4 = sn3 * sn;

            /* term1 = (i-1)*(6*i^3 - 25*i^2 + 33*i - 12)*a[i-1] */
            seq_t coeff1 = (seq_t)(i-1) *
                           ((seq_t)6 * sn3 - (seq_t)25 * sn2 + (seq_t)33 * sn - (seq_t)12);
            seq_t term1 = cache[i-1] * coeff1;

            /* term2 = (i-2)^3*(6*i^3 - 29*i^2 + 42*i - 15)*a[i-2] */
            seq_t coeff2 = (seq_t)(i-2) * (seq_t)(i-2) * (seq_t)(i-2) *
                           ((seq_t)6 * sn3 - (seq_t)29 * sn2 + (seq_t)42 * sn - (seq_t)15);
            seq_t term2 = cache[i-2] * coeff2;

            /* term3 = (i-3)^4*(i-2)^3*(2*i -1)*a[i-3] */
            seq_t coeff3 = (seq_t)(i-3) * (seq_t)(i-3) * (seq_t)(i-3) * (seq_t)(i-3) *
                           (seq_t)(i-2) * (seq_t)(i-2) * (seq_t)(i-2) *
                           ((seq_t)2 * sn - (seq_t)1);
            seq_t term3 = cache[i-3] * coeff3;

            /* numerator = term1 + term3 - term2   (to avoid unsigned underflow) */
            seq_t numerator = term1 + term3;
            numerator = numerator - term2;

            seq_t denominator = (seq_t)i * ((seq_t)2 * sn - (seq_t)3);
            cache[i] = numerator / denominator;
        }
    }

    return cache[n];
}
