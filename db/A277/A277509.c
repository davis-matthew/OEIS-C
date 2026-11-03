/*
 * A277509  Expansion of e.g.f. 1/((1+LambertW(-x))*(1+x)).
 * https://oeis.org/A277509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0, a(n) = n^n - n*a(n-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 384 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t capacity = 0;   /* allocated size of cache */
    static idx_t  computed = 0;   /* number of terms actually computed   */

    /* grow cache to hold up to index n */
    if (n >= computed) {
        size_t new_cap = (size_t)n + 1;
        if (cache == NULL) {
            cache = seq_alloc(new_cap);
        } else if (new_cap > capacity) {
            cache = seq_realloc(cache, capacity, new_cap);
        }
        capacity = new_cap;

        /* initialise the first term if needed */
        if (computed == 0) {
            cache[0] = 1;        /* a(0) = 1 */
            computed = 1;
        }

        /* compute missing terms from computed up to n      */
        for (idx_t i = computed; i <= n; ++i) {
            seq_t term = ipow_idx((seq_t)i, i);          /* i^i   */
            seq_t prod = mul_small(cache[i-1], (unsigned int)i); /* n*a(n-1) */
            cache[i]   = term - prod;                  /* a(n)  */
        }
        computed = n + 1;
    }

    return cache[n];
}
