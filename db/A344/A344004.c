/*
 * A344004  Number of ordered subsequences of {1,...,n} containing at least
 *          three elements and such that the first differences contain only
 *          odd numbers.
 * https://oeis.org/A344004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = 3*a(n-1) - a(n-2) -
 *            4*a(n-3) + 3*a(n-4) + a(n-5) - a(n-6) for n ≥ 7, with a(1)=0,
 *            a(2)=0, a(3)=1, a(4)=3, a(5)=8, a(6)=17. The sequence starts
 *            a(1)=0, a(2)=0, a(3)=1, a(4)=3, … The code memoises computed
 *            values for efficiency
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifndef __cplusplus
/* C build: use malloc/realloc for the cache */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
#endif
static const seq_t init_vals[7] = {0,0,0,1,3,8,17};   /* init_vals[n] for 1≤n≤6 */

#ifdef __cplusplus
#include <vector>
#endif

/* access macros that work for both C and C++ builds */
#ifdef __cplusplus
#define CACHE_GET(i) (vec[i])
#define CACHE_SET(i,val) (vec[i] = (val))
#else
#define CACHE_GET(i) (cache[i])
#define CACHE_SET(i,val) (cache[i] = (val))
#endif

static idx_t last_calculated = 0;          /* highest index already computed */
static int cache_initialized = 0;          /* whether base values are in cache */

#ifdef __cplusplus
/* C++ build: use std::vector to store the cache */
static std::vector<seq_t> vec;
#endif

/* Ensure the cache can hold indices up to n (inclusive). */
static void
ensure_cache_cap(idx_t n)
{
#ifdef __cplusplus
    if (vec.size() >= n+1) return;
    vec.resize(n+1);
#else
    if (cache_cap >= n) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < n) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
#endif
}

/* Compute a(n). */
seq_t
a(idx_t n)
{
    if (n <= 6) return init_vals[n];

    ensure_cache_cap(n);

    if (!cache_initialized) {
        for (idx_t i = 1; i <= 6; ++i) CACHE_SET(i, init_vals[i]);
        last_calculated = 6;
        cache_initialized = 1;
    }

    /* compute values up to n if not already done */
    for (idx_t i = last_calculated + 1; i <= n; ++i) {
        seq_t term = 3 * CACHE_GET(i-1);         /* 3*a(i-1) */
        term -= CACHE_GET(i-2);                 /* - a(i-2) */
        term -= 4 * CACHE_GET(i-3);             /* -4*a(i-3) */
        term += 3 * CACHE_GET(i-4);             /* +3*a(i-4) */
        term += CACHE_GET(i-5);                 /* + a(i-5) */
        term -= CACHE_GET(i-6);                 /* - a(i-6) */
        CACHE_SET(i, term);
    }
    last_calculated = n;
    return CACHE_GET(n);
}
