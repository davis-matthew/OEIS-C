/*
 * A397990  Upper (1,1/4) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A397990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + a(n-4) Initial values:
 *            a(0)=0,a(1)=2,a(2)=4,a(3)=9 Computation is memoised so calls may
 *            be in arbitrary order
 * Range:     u8 n <= 18, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Local cache */
#if defined(__cplusplus)
/* C++ implementation: use std::vector for automatic construction/destruction */
static std::vector<seq_t> cache;
#else
/* C implementation: use raw memory; seq_t must be POD or have trivial init */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size */
static size_t cache_size = 0;  /* number of elements filled */
#endif

seq_t a(idx_t n)
{
    /* Ensure the cache can hold the requested index */
#if defined(__cplusplus)
    if (cache.size() <= (size_t)n) {
        cache.resize((size_t)n + 1);
    }
    /* Compute missing entries */
    for (size_t i = cache.size() - 1; i <= (size_t)n; ++i) { /* empty when already computed */ }
    for (size_t i = 0; i <= (size_t)n; ++i) {
        if (i == 0) { cache[0] = 0; }
        else if (i == 1) { cache[1] = 2; }
        else if (i == 2) { cache[2] = 4; }
        else if (i == 3) { cache[3] = 9; }
        else if (i >= 4 && cache[i] == 0) { /* values may legitimately be zero; use separate flag if needed */
            cache[i] = 2 * cache[i-1] - 2 * cache[i-3] + cache[i-4];
        }
    }
    return cache[(size_t)n];
#else
    /* Expand allocation if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    /* Compute missing entries */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i == 0) { cache[0] = 0; }
        else if (i == 1) { cache[1] = 2; }
        else if (i == 2) { cache[2] = 4; }
        else if (i == 3) { cache[3] = 9; }
        else { cache[i] = 2 * cache[i-1] - 2 * cache[i-3] + cache[i-4]; }
    }
    if (cache_size <= (size_t)n) cache_size = (size_t)n + 1;
    return cache[(size_t)n];
#endif
}
