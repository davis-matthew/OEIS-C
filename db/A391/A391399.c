/*
 * A391399  a(n) = Sum_{k=0..floor(n/3)} binomial(k,2*n-6*k).
 * https://oeis.org/A391399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(k , 2*n-6*k) * Recurrence
 *            (used for fast computation): * a(n) = 2*a(n-3) - a(n-6) + a(n-7)
 *            * a(0)=1, a(1)=0, a(2)=0, a(3)=1, a(4)=0, a(5)=0, a(6)=1, a(7)=1
 *            * The function memoizes results and works for any order of calls
 * Range:     u8 n <= 34, u16 n <= 60, u32 n <= 112, u64 n <= 216,
 *            u128 n <= 423, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* memoization cache */
static size_t cache_cap = 0;    /* allocated capacity */
static size_t cache_size = 0;   /* number of elements stored */

static void grow_cache(idx_t min_needed)
{
    size_t new_cap = cache_cap > 0 ? cache_cap : 4;
    while (new_cap <= (size_t)min_needed) new_cap <<= 1;

    seq_t *new_arr;
#ifdef __cplusplus
    new_arr = new seq_t[new_cap];
#else
    new_arr = (seq_t *)calloc(new_cap, sizeof(seq_t));
#endif
    for (size_t i = 0; i < cache_size; ++i) {
        new_arr[i] = cache[i];
    }
#ifdef __cplusplus
    delete[] cache;
#else
    free(cache);
#endif
    cache = new_arr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure cache exists and large enough */
    if (!cache || cache_cap <= (size_t)n) {
        if (!cache) {
            cache_cap = 64;
#ifdef __cplusplus
            cache = new seq_t[cache_cap];
#else
            cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
#endif
            if (!cache) exit(1);
            cache_size = 0;
        }
        while (cache_cap <= (size_t)n) grow_cache((idx_t)cache_cap);
    }

    /* Pay attention to calls with n < 7: initialise base values */
    if (cache_size <= 6) {
        for (idx_t i = cache_size; i <= 6; ++i) {
            switch (i) {
                case 0: cache[0] = (seq_t)1; break;
                case 1: case 2: cache[i] = (seq_t)0; break;
                case 3: case 6: cache[i] = (seq_t)1; break;
                case 4: case 5: cache[i] = (seq_t)0; break;
            }
        }
        cache_size = 7;
    }

    /* Return early if already computed */
    if (n < cache_size) return cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = cache[i-3];
        val += cache[i-3];        /* 2*a(i-3) */
        val += cache[i-7];        /* + a(i-7) */
        val -= cache[i-6];        /* - a(i-6) */
        cache[i] = val;
    }
    cache_size = n + 1;
    return cache[n];
}
