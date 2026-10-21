/*
 * A276968  Odd integers n such that 2^n == 2^5 (mod n).
 * https://oeis.org/A276968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th odd integer m such that 2^m ≡ 32 (mod m)
 * Range:     u8 n <= 11, u16 n <= 844, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by testing odd integers in increasing order and storing
   those that satisfy 2^m ≡ 32 (mod m). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;  /* array of sequence terms */
    static size_t cache_len = 0; /* number of terms stored */
    static size_t cache_cap = 0; /* current capacity */
    static uint64_t next_candidate = 1; /* next odd integer to test */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = seq_alloc(cache_cap); /* allocates cache_cap elements of type seq_t */
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* compute 2^next_candidate (mod next_candidate) */
        seq_t rem = mod_pow((seq_t)2, (uint64_t)next_candidate, next_candidate);

        /* target value is 32 mod next_candidate */
        seq_t target = ((seq_t)32) % next_candidate;

        if (rem == target) { /* candidate satisfies the condition */
            if (cache_len == cache_cap) { /* grow array if needed */
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }

        /* advance to next odd integer */
        if (next_candidate == 1)
            next_candidate = 3;
        else
            next_candidate += 2;
    }

    return cache[n - 1];
}
