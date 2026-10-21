/*
 * A250850  Number of (n+1) X (5+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 35*a(n-2) + 50*a(n-3) - 24*a(n-4); * Initial
 *            values: a(1)=7056, a(2)=49599, a(3)=290646, a(4)=1499679
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 25, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cached values of the sequence, 1‑based */
static idx_t cache_len = 0;   /* number of terms stored in cache */

/* Helper: construct objects when compiled as C++ */
#ifdef __cplusplus
#include <new>
#endif

static void construct_cache_element(seq_t *p)
{
#ifdef __cplusplus
    new (p) seq_t();
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, return 0 as guard */

    /* Ensure we have the first four terms in the cache */
    if (!cache) {
        cache_len = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return 0; /* allocation failure guard */
        for (idx_t i = 0; i < cache_len; ++i)
            construct_cache_element(cache + i);
        cache[0] = 7056;   /* a(1) */
        cache[1] = 49599;  /* a(2) */
        cache[2] = 290646; /* a(3) */
        cache[3] = 1499679;/* a(4) */
    }

    /* If the requested n is larger than what we have, extend the cache */
    if (cache_len < n) {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!new_cache) return 0; /* allocation failure guard */
        for (idx_t i = cache_len; i < n; ++i)
            construct_cache_element(new_cache + i);
        for (idx_t i = cache_len; i < n; ++i) {
            /* idx i refers to a(i+1) because cache is 0‑based */
            seq_t val = 10 * new_cache[i-1]
                      - 35 * new_cache[i-2]
                      + 50 * new_cache[i-3]
                      - 24 * new_cache[i-4];
            new_cache[i] = val;
        }
        cache = new_cache;
        cache_len = n;
    }

    return cache[n-1];  /* a(n) */
}
