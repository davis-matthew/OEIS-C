/*
 * A378701  Number of minimal edge covers in the n-Plummer-Toft graph.
 * https://oeis.org/A378701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            4*a(n-1)+a(n-2)-12*a(n-3)-15*a(n-4)+24*a(n-5)+49*a(n-6)+6*a(n-7)-73*a(n-8)-76*a(n-9)+5*a(n-10)+80*a(n-11)+72*a(n-12)+14*a(n-13)-30*a(n-14)-34*a(n-15)-19*a(n-16)-6*a(n-17)-a(n-18)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 45, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values a(1)..a(18) as given. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[18] = {
        25ULL, 66ULL, 181ULL, 456ULL, 1075ULL,
        2579ULL, 6082ULL, 14414ULL, 34450ULL, 82895ULL,
        201315ULL, 492855ULL, 1215426ULL, 3016505ULL, 7526261ULL,
        18860558ULL, 47431375ULL, 119619948ULL
    };

    /* Direct lookup for the first 18 terms. */
    if (n <= 18) {
        return init[n - 1];
    }

    /* Static cache for memoization. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Ensure cache has at least 18 computed terms. */
    if (cache_len < 18) {
        size_t cap = (n > 18) ? n : 18;
        seq_t *tmp = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!tmp) abort();
        for (idx_t i = 0; i < 18; ++i) {
            tmp[i] = init[i];
        }
        cache = tmp;
        cache_len = 18;
    }

    /* Grow cache if necessary. */
    if (n > cache_len) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t idx = cache_len; idx < n; ++idx) {
        seq_t pos = 0, neg = 0;
        pos += cache[idx - 1] * (seq_t)4;
        pos += cache[idx - 2];                /* coefficient 1 */
        neg += cache[idx - 3] * (seq_t)12;
        neg += cache[idx - 4] * (seq_t)15;
        pos += cache[idx - 5] * (seq_t)24;
        pos += cache[idx - 6] * (seq_t)49;
        pos += cache[idx - 7] * (seq_t)6;
        neg += cache[idx - 8] * (seq_t)73;
        neg += cache[idx - 9] * (seq_t)76;
        pos += cache[idx -10] * (seq_t)5;
        pos += cache[idx -11] * (seq_t)80;
        pos += cache[idx -12] * (seq_t)72;
        pos += cache[idx -13] * (seq_t)14;
        neg += cache[idx -14] * (seq_t)30;
        neg += cache[idx -15] * (seq_t)34;
        neg += cache[idx -16] * (seq_t)19;
        neg += cache[idx -17] * (seq_t)6;
        neg += cache[idx -18];                /* coefficient 1 */
        cache[idx] = pos - neg;
    }
    cache_len = n;

    return cache[n - 1];
}
