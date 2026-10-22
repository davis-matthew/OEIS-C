/*
 * A332772  Numbers k > 0 such that 30k +- 7 is prime.
 * https://oeis.org/A332772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A332772: Numbers k > 0 such that 30k ± 7 is prime.
   The function returns the n-th such k (1‑based).
   It builds the sequence lazily and memoises results, so calls may come
   in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime_big.h"   /* is_prime_big(seq_t) */

seq_t a(idx_t n)
{
    /* static cache of found k values */
    static seq_t *cache = NULL;      /* holds the k's */
    static size_t cache_cnt = 0;     /* number of elements stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t next_k = 1;         /* next k to test */

    if (n == 0) /* the off‑by‑one offset, not used in OEIS */
        return 0;

    /* grow the cache until we have at least n elements */
    while (cache_cnt < (size_t)n) {
        /* find the next k with both 30k-7 and 30k+7 prime */
        for (;;) {
            uint64_t v1 = 30u * (uint64_t)next_k - 7u;
            uint64_t v2 = 30u * (uint64_t)next_k + 7u;

            if (is_prime_big((seq_t)v1) && is_prime_big((seq_t)v2)) {
                /* store the found k */
                if (cache_cnt == cache_cap) {
                    cache_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                    cache = (seq_t *)realloc(cache,
                                              cache_cap * sizeof(seq_t));
                }
                cache[cache_cnt++] = (seq_t)next_k;
                ++next_k;
                break;          /* move on to the next needed term */
            }
            ++next_k;
        }
    }

    return cache[(size_t)n - 1];
}
