/*
 * A393043  Numbers such that the least prime not dividing their arithmetic
 *          derivative is 3.
 * https://oeis.org/A393043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: numbers whose arithmetic derivative is even but not divisible by 3. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Static cache for terms, indexed from 1 */
    static seq_t *cache = NULL;      /* 1‑based array of terms */
    static size_t  cache_alloc = 0;  /* allocated length of cache   */
    static size_t  cache_len   = 0;  /* number of elements stored   */
    static uint64_t next_candidate = 1; /* next integer to test       */

    /* Allocate initial cache if not yet */
    if (!cache) {
        cache_alloc = 64;
        cache = (seq_t *)malloc(cache_alloc * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out of memory – safe return */
        cache[0] = 0;                  /* dummy element for 1‑based indexing */
    }

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n];

    /* Temporary arrays for prime factorisation */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t   cnt;

    /* Generate terms until we have at least n terms */
    while (cache_len < n) {
        /* Skip 1, as its derivative is 0 (not even/odd issues) */
        uint64_t cand = ++next_candidate;
        if (cand <= 1)
            continue;

        /* Factorise cand */
        factor_u64(cand, pr, ex, &cnt);

        /* Compute arithmetic derivative:
           n' = Σ e_i * (n / p_i)   (exact integer)          */
        __uint128_t der = 0;
        for (size_t i = 0; i < cnt; ++i) {
            der += (__uint128_t)ex[i] * (cand / pr[i]);
        }

        /* Condition: derivative is even (divisible by 2)
           but not divisible by 3.                       */
        if ( ((der & 1) == 0) && ((der % 3) != 0) ) {
            /* Ensure cache capacity */
            if (cache_len + 1 >= cache_alloc) {
                size_t new_alloc = cache_alloc * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* out of memory */
                cache = tmp;
                cache_alloc = new_alloc;
            }
            cache[cache_len + 1] = (seq_t)cand;
            cache_len++;
        }
    }

    return cache[n];
}
