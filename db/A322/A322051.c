/*
 * A322051  a(n) is the number of initial terms in the row of length 2^n of
 *          A322050 that agree with the limiting sequence A322049.
 * https://oeis.org/A322051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=4. For n ≥ 5: a(n) = 2 * a(n-1) if
 *            n is even a(n) = 2 * a(n-1) - 1 if n is odd
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;   /* array of computed a(i) */
    static idx_t cached_n = (idx_t)-1; /* largest n computed so far */
    static size_t cache_cap = 0;       /* allocated capacity of cache */

    /* initialise cache on first call */
    if (cached_n == (idx_t)-1) {
        /* allocate initial capacity */
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;  /* out‑of‑memory; caller should handle */
        /* set the known small values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;
        for (idx_t i = 4; i < cache_cap; ++i)
            cache[i] = (seq_t)0;
        cached_n = 3;
    }

    /* grow cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)(n + 1) > new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* out‑of‑memory */
        cache = tmp;
        for (idx_t i = cache_cap; i < new_cap; ++i)
            cache[i] = (seq_t)0;
        cache_cap = new_cap;
    }

    /* if already computed, just return */
    if (n <= cached_n)
        return cache[n];

    /* compute terms up to n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t val;
        if (i >= 5) {
            if ((i & 1U) == 0) {        /* even */
                val = (seq_t)2 * cache[i-1];
            } else {                    /* odd */
                val = (seq_t)2 * cache[i-1] - (seq_t)1;
            }
        } else {
            /* use pre‑set values for 0..4 */
            switch (i) {
                case 0: val = (seq_t)1; break;
                case 1: val = (seq_t)1; break;
                case 2: val = (seq_t)2; break;
                case 3: val = (seq_t)4; break;
                case 4: val = (seq_t)6; break; /* from the recurrence */
            }
        }
        cache[i] = val;
    }
    cached_n = n;
    return cache[n];
}
