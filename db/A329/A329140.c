/*
 * A329140  Numbers whose prime signature is a periodic word.
 * https://oeis.org/A329140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_periodic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A329140: Numbers whose prime signature is a periodic word.
   The function computes the nth such integer by brute‑force search,
   factoring each candidate by trial division.  Results are cached
   so subsequent calls are fast regardless of order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_periodic.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Static cache for already-found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Next integer to examine. */
    static uint64_t cand = 2;

    /* Allocate initial cache. */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Produce terms until cache holds at least n of them. */
    size_t need = (size_t)n;
    while (cache_len < need) {

        /* Factor cand by trial division. */
        uint64_t x = cand;
        int exps[64];
        size_t ecnt = 0;

        /* Handle factor 2. */
        if (x % 2 == 0) {
            int e = 0;
            while (x % 2 == 0) {
                x /= 2;
                e++;
            }
            exps[ecnt++] = e;
        }

        /* Handle odd factors. */
        for (uint64_t p = 3; p * p <= x; p += 2) {
            if (x % p == 0) {
                int e = 0;
                while (x % p == 0) {
                    x /= p;
                    e++;
                }
                exps[ecnt++] = e;
            }
        }

        /* Remaining prime factor, if any. */
        if (x > 1) {
            exps[ecnt++] = 1;
        }

        /* Determine if this number qualifies. */
        if (is_periodic(exps, ecnt)) {
            /* Store the candidate in cache. */
            if (cache_len >= cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) { /* OOM: abort gracefully. */
                    free(cache);
                    cache = NULL;
                    cache_cap = cache_len = 0;
                    return (seq_t)0;
                }
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)cand;
        }

        ++cand;
    }

    /* Return the cached term at index n (1‑based). */
    return cache[need - 1];
}
