/*
 * A314236  Coordination sequence Gal.6.626.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 3*a(n-3) - a(n-4) - a(n-6) +
 *            3*a(n-7) - 4*a(n-8) + 3*a(n-9) - a(n-10) (for n > 10) Initial
 *            values: a(0)=1, a(1)=5, a(2)=11, a(3)=17, a(4)=23, a(5)=28,
 *            a(6)=33, a(7)=38, a(8)=43, a(9)=49, a(10)=55
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to store already computed terms */
    static seq_t *cache = NULL;
    static size_t cap    = 0;   /* allocated capacity */
    static size_t len    = 0;   /* number of terms stored */

    /* initial values for the first 11 terms */
    static const seq_t init[11] = { 1, 5, 11, 17, 23, 28, 33, 38, 43, 49, 55 };

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 16;                    /* start with a small buffer */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* out of memory, unlikely */

        /* copy the initial terms */
        for (idx_t i = 0; i < 11; ++i) cache[i] = init[i];
        len = 11;
    }

    /* direct return if the term is already cached */
    if (n < len) return cache[n];

    /* enlarge the cache if necessary */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* compute terms from len up to n using the recurrence */
    for (idx_t i = len; i <= n; ++i) {
        /* compute positive and negative parts separately to avoid unsigned underflow */
        seq_t pos = 3 * cache[i - 1] + 3 * cache[i - 3] + 3 * cache[i - 7] + 3 * cache[i - 9];
        seq_t neg = 4 * cache[i - 2] +       cache[i - 4] +   cache[i - 6] + 4 * cache[i - 8] +      cache[i -10];
        cache[i] = pos - neg;   /* pos >= neg for all n, result fits in seq_t */
    }

    len = (size_t)n + 1;
    return cache[n];
}
