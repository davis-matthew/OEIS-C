/*
 * A281454  Exponential transform of the 4-dimensional figurate numbers
 *          (A002417).
 * https://oeis.org/A281454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n) = ∑_{k=1}^n C(n-1,k-1) · k · (6 + 18(k-1) +
 *            9(k-1)(k-2) + (k-1)(k-2)(k-3)) / 6
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses memoised dynamic programming. It works for
   any seq_t/idx_t combination as long as seq_t supports the usual
   arithmetic operators and integer division. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached sequence values */
    static idx_t cache_size = 0;  /* number of entries currently stored */
    static size_t alloc_cap = 0;   /* allocated capacity */

    /* initialise cache for the first call */
    if (cache == NULL) {
        alloc_cap   = (n + 1) > 1 ? (n + 1) : 1;
        cache       = (seq_t*)malloc(alloc_cap * sizeof(seq_t));
        if (!cache) return 0;                /* out of memory */
        cache[0] = (seq_t)1;                 /* a(0) = 1 */
        cache_size = 1;
    }

    /* grow the allocation if necessary */
    if ((idx_t)(n + 1) > alloc_cap) {
        size_t new_cap = alloc_cap * 2;
        if ((size_t)(n + 1) > new_cap) new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!tmp) return 0;                  /* out of memory */
        for (idx_t i = 0; i < cache_size; ++i) tmp[i] = cache[i];
        free(cache);
        cache = tmp;
        alloc_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t m = cache_size; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t comb = (seq_t)1;   /* C(m-1, k-1) */
        for (idx_t k = 1; k <= m; ++k) {
            if (k > 1) {
                /* update binomial coefficient incrementally */
                comb = comb * (seq_t)(m - (k - 1));
                comb = comb / (seq_t)(k - 1);
            }

            idx_t t = k - 1;
            seq_t tt = (seq_t)t;
            seq_t num = (seq_t)6;
            num += (seq_t)18 * tt;
            num += (seq_t)9 * tt * (tt - (seq_t)1);
            if (k >= 4) {
                num += tt * (tt - (seq_t)1) * (tt - (seq_t)2);
            }

            seq_t term = cache[m - k] * comb;
            term = term * (seq_t)k;
            term = term * num;
            term = term / (seq_t)6;  /* guaranteed integer division */

            sum += term;
        }
        cache[m] = sum;
        ++cache_size;
    }

    return cache[n];
}
