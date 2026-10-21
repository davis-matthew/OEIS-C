/*
 * A271487  Maximal term of TRIP-Stern sequence of level n corresponding to
 *          permutation triple (e,13,132).
 * https://oeis.org/A271487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*a(n-3) + 2*a(n-6) + a(n-9) for n>9 with initial values
 *            a(0)=1, a(1)=2, a(2)=3, a(3)=4, a(4)=6, a(5)=8, a(6)=11,
 *            a(7)=17, a(8)=23, a(9)=32
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* C implementation */
#ifndef __cplusplus
    /* Static state shared between calls */
    static seq_t *cache = NULL;   /* array holding computed terms */
    static size_t cap  = 0;        /* allocated capacity of cache */
    static idx_t  len  = 0;        /* first index not yet computed */
    static bool   init = false;   /* have the first 10 terms been set? */

    /* ensure cache can hold indices up to n */
    if (cap <= (size_t)n) {
        size_t new_cap = cap == 0 ? 16 : cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;          /* double until enough */
        seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise the first ten terms only once */
    if (!init) {
        size_t need = 10;
        if (cap < need) {
            size_t new_cap = cap == 0 ? 16 : cap;
            while (new_cap < need) new_cap <<= 1;
            seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cap   = new_cap;
        }
        cache[0]  = (seq_t)1;  cache[1]  = (seq_t)2;
        cache[2]  = (seq_t)3;  cache[3]  = (seq_t)4;
        cache[4]  = (seq_t)6;  cache[5]  = (seq_t)8;
        cache[6]  = (seq_t)11; cache[7]  = (seq_t)17;
        cache[8]  = (seq_t)23; cache[9]  = (seq_t)32;
        len      = 10;
        init     = true;
    }

    /* return if already computed */
    if (n < len)
        return cache[n];

    /* compute missing terms using the recurrence */
    for (idx_t i = len; i <= n; ++i) {
        cache[i] = 2 * cache[i - 3] + 2 * cache[i - 6] + cache[i - 9];
    }
    len = n + 1;
    return cache[n];
#endif /* __cplusplus */

    /* C++ implementation (uses std::vector for arbitrary‑precision types) */
#ifdef __cplusplus
    static std::vector<seq_t> cache;

    /* initialise the first ten terms only once */
    if (cache.empty()) {
        cache.resize(10);
        cache[0]  = (seq_t)1;  cache[1]  = (seq_t)2;
        cache[2]  = (seq_t)3;  cache[3]  = (seq_t)4;
        cache[4]  = (seq_t)6;  cache[5]  = (seq_t)8;
        cache[6]  = (seq_t)11; cache[7]  = (seq_t)17;
        cache[8]  = (seq_t)23; cache[9]  = (seq_t)32;
    }

    /* return if already computed */
    if (n < cache.size())
        return cache[n];

    /* compute missing terms */
    idx_t start = cache.size();
    cache.resize(n + 1);
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = 2 * cache[i - 3] + 2 * cache[i - 6] + cache[i - 9];
    }
    return cache[n];
#endif /* __cplusplus */
}
