/*
 * A275179  Number of n X 4 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-1,0) or (-1,1) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n‑1) – 28*a(n‑2) + 57*a(n‑3) – 70*a(n‑4) + 47*a(n‑5)
 *            – 12*a(n‑6) – a(n‑7) – 2*a(n‑8) + a(n‑9) (n > 12) Initial values
 *            (n = 1 … 12): 14, 64, 89, 141, 353, 914, 2386, 6228, 16249,
 *            42451, 111108, 291065
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   allocate_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: For n < 1 the function returns 0 . */

#include <stdlib.h>
#include "../utils/allocate_cache.h"

static void free_cache(seq_t *p)
{
#if defined(__cplusplus)
    delete[] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    if (n == 0)                       /* offset is 1 */
        return 0;

    /* initial terms (up to n = 12) as given in the OEIS entry */
    static const seq_t init[12] = {14, 64, 89, 141, 353, 914,
                                   2386, 6228, 16249, 42451,
                                   111108, 291065};

    if (n <= 12)
        return init[n - 1];

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;    /* number of entries already stored (>=12 after init) */
    static size_t cache_cap  = 0;    /* allocated capacity */

    /* first allocation or a request larger than the current capacity */
    if (cache == NULL) {
        size_t cap = n > 256 ? n : 256;
        cache = allocate_cache(cap);
        if (!cache) exit(1);                 /* fatal if allocation fails */
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init[i];
        cached_len = 12;
        cache_cap  = cap;
    } else if (cache_cap < n) {
        size_t newcap = cache_cap * 2;
        while (newcap < n) newcap <<= 1;
        seq_t *tmp = allocate_cache(newcap);
        if (!tmp) exit(1);
        for (size_t i = 0; i < cached_len; ++i)
            tmp[i] = cache[i];
        free_cache(cache);
        cache = tmp;
        cache_cap = newcap;
    }

    /* compute new terms up to index n (zero‑based index i = n-1) */
    for (idx_t i = cached_len; i < n; ++i) {
        /* i >= 12 here, so all indices i-1 … i-9 are valid */
        seq_t t =  (seq_t)8     * cache[i-1]
                 - (seq_t)28    * cache[i-2]
                 + (seq_t)57    * cache[i-3]
                 - (seq_t)70    * cache[i-4]
                 + (seq_t)47    * cache[i-5]
                 - (seq_t)12    * cache[i-6]
                 -              cache[i-7]
                 - (seq_t)2     * cache[i-8]
                 +              cache[i-9];
        cache[i] = t;
    }
    cached_len = n;

    return cache[n - 1];
}
