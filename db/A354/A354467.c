/*
 * A354467  Positive integers whose prime factors are congruent to 1 (mod 12).
 * https://oeis.org/A354467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer whose prime factors are all ≡ 1
 *            (mod 12)
 * Range:     u8 n <= 13, u16 n <= 2011, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factors_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factors_ok.h"

/* ------------------------------------------------------------------ */
/* Helper: test whether all prime factors of x are ≡ 1 (mod 12)         */

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_candidate = 2;   /* next integer to test */

    if (!cache) {                     /* first call – initialise cache */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[cache_len++] = (seq_t)1;  /* a(1) = 1 */
    }

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (factors_ok(next_candidate)) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[(size_t)n - 1];
}
