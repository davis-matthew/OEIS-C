/*
 * A373644  a(n) = Sum_{k=0..floor(n/4)} binomial(2*n-7*k,k).
 * https://oeis.org/A373644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊n/4⌋} binomial(2*n-7*k,k); Recurrence (for n >
 *            7): a(n) = a(n-1) + 2*a(n-4) - a(n-8); Initial terms:
 *            a(0)=1,a(1)=1,a(2)=1,a(3)=1,a(4)=2,a(5)=4,a(6)=6,a(7)=8
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cached sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* current allocated size */
static size_t cache_len = 0;   /* number of elements already computed */

/* Initial values for n = 0..7 */
static const seq_t init_vals[8] = {1,1,1,1,2,4,6,8};

seq_t a(idx_t n) {
    /* Ensure the cache is large enough to hold index n */
    size_t need = (size_t)n + 1;
    if (cache == NULL) {
#ifdef __cplusplus
        cache = new seq_t[need];
#else
        cache = (seq_t *)malloc(need * sizeof(seq_t));
#endif
        if (cache == NULL) {
            /* In a real program you would handle OOM here. */
            return 0;
        }
        cache_cap = need;
    } else if (cache_cap < need) {
#ifdef __cplusplus
        seq_t *tmp = new seq_t[need];
        for (size_t i = 0; i < cache_cap; ++i) tmp[i] = cache[i];
        delete[] cache;
        cache = tmp;
#else
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        cache = tmp;
        if (cache == NULL) {
            /* OOM */
            return 0;
        }
#endif
        cache_cap = need;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i <= n; ++i) {
        if (i < 8) {
            cache[i] = init_vals[i];
        } else {
            /* a[i] = a[i-1] + 2*a[i-4] - a[i-8] */
            seq_t double_prev4 = cache[i-4] + cache[i-4];
            seq_t tmp = cache[i-1] + double_prev4;
            cache[i] = tmp - cache[i-8];
        }
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
