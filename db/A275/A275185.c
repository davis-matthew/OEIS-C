/*
 * A275185  Number of 5 X n 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-1,0) or (-1,1) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) + 8*a(n-3) for n > 5 a(1)=8, a(2)=21,
 *            a(3)=85, a(4)=353, a(5)=1465
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses memoisation to handle arbitrary order of calls
   and works for all supported seq_t and idx_t types. */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* --------------------------------------------------------------------- */
/* Global state for memoisation                                        */
/* --------------------------------------------------------------------- */
#ifndef __cplusplus
static seq_t *cache = NULL;      /* array indexed from 1 (0 unused)      */
static idx_t cache_size = 0;    /* highest index computed so far       */
static idx_t cache_cap  = 0;    /* allocated capacity (>= cache_size)  */
#else
/* In C++ we use a std::vector for dynamic growth. */
static std::vector<seq_t> cache_vec;
static idx_t cache_size = 0;    /* highest index computed so far       */
#endif

/* --------------------------------------------------------------------- */
/* Utility: ensure we have enough capacity in the C version             */
/* --------------------------------------------------------------------- */
#ifndef __cplusplus
static void ensure_capacity(idx_t n)
{
    if (n <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < n) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}
#endif

/* --------------------------------------------------------------------- */
/* Initialise base values (a(1) .. a(5)).                               */
/* --------------------------------------------------------------------- */
static void init_cache(void)
{
#ifndef __cplusplus
    if (cache) return;                     /* already initialised */
    ensure_capacity(5);
    cache[0] = (seq_t)0;                   /* index 0 unused       */
    cache[1] = (seq_t)8;
    cache[2] = (seq_t)21;
    cache[3] = (seq_t)85;
    cache[4] = (seq_t)353;
    cache[5] = (seq_t)1465;
    cache_size = 5;
#else
    if (!cache_vec.empty()) return;         /* already initialised */
    cache_vec.resize(6);                    /* indices 0..5, 0 unused */
    cache_vec[0] = (seq_t)0;
    cache_vec[1] = (seq_t)8;
    cache_vec[2] = (seq_t)21;
    cache_vec[3] = (seq_t)85;
    cache_vec[4] = (seq_t)353;
    cache_vec[5] = (seq_t)1465;
    cache_size = 5;
#endif
}

/* --------------------------------------------------------------------- */
/* Public function                                                     */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* safety; sequence defined for n>=1 */
    init_cache();

#ifndef __cplusplus
    if (n > cache_size) {
        ensure_capacity(n);
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            cache[i] = (seq_t)2 * cache[i-1]
                     + (seq_t)7 * cache[i-2]
                     + (seq_t)8 * cache[i-3];
        }
        cache_size = n;
    }
    return cache[n];
#else
    if (n > cache_size) {
        cache_vec.resize(n + 1);            /* enlarge vector, reserve slot 0..n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            cache_vec[i] = (seq_t)2 * cache_vec[i-1]
                         + (seq_t)7 * cache_vec[i-2]
                         + (seq_t)8 * cache_vec[i-3];
        }
        cache_size = n;
    }
    return cache_vec[n];
#endif
}
