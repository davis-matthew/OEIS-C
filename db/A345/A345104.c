/*
 * A345104  a(n) = 1 + 2 * Sum_{k=0..n-1} binomial(n-1,k) * a(k) * a(n-k-1).
 * https://oeis.org/A345104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 2 * Sum_{k=0..n-1} C(n-1,k) * a(k) * a(n-k-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS sequence A345104) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

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

/*  Cache for computed values */
static seq_t *cache     = NULL;   /* buffer of a(0)..a(len-1)          */
static idx_t  cache_len = 0;      /* number of elements currently stored */
static idx_t  cache_cap = 0;      /* capacity of the buffer             */

/* Ensure the cache can hold at least n+1 elements. */
static void ensure_cache_capacity(idx_t n)
{
    if (n + 1 > cache_cap) {
        seq_t *tmp = seq_realloc(cache, cache_cap, n + 1);
        if (!tmp) {
            /* Out of memory – the framework will abort the program. */
            abort();
        }
        cache    = tmp;
        cache_cap = n + 1;
    }
}

seq_t a(idx_t n)
{
    /* First term a(0) = 1 is handled automatically below. */
    ensure_cache_capacity(n);

    /* Compute all values from the first missing index up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        idx_t m   = i - 1;    /* we need C(m,k) for k = 0..m */
        seq_t bin = (seq_t)1; /* C(m,0) */

        for (idx_t k = 0; k <= m; ++k) {
            /* term = C(m,k) * a(k) * a(m-k) */
            seq_t term = bin * cache[k];
            term *= cache[m - k];
            sum += term;

            if (k < m) {
                /* Update binomial coefficient for next k:
                   C(m,k+1) = C(m,k) * (m-k) / (k+1) */
                seq_t num = bin * (seq_t)(m - k);
                bin = num / (seq_t)(k + 1);
            }
        }

        cache[i] = (seq_t)1 + (seq_t)2 * sum;
    }

    cache_len = n + 1;
    return cache[n];
}
