/*
 * A259730  Primes p such that both 2*p - 3 and 3*p - 2 are prime.
 * https://oeis.org/A259730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth prime p such that 2*p-3 and 3*p-2 are also prime
 * Range:     u8 n <= 20, u16 n <= 497, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once

#include <stddef.h>
#include <stdbool.h>

#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"

/* Grow a dynamic array of seq_t elements. */
static void ensure_capacity(size_t needed, seq_t **p_cache, size_t *p_cap) {
    size_t cap = *p_cap;
    if (cap >= needed) return;
    size_t new_cap = cap ? (cap << 1) : 8;
    while (new_cap < needed) new_cap <<= 1;
    *p_cache = seq_realloc(*p_cache, cap, new_cap);
    *p_cap = new_cap;
}

/* Return the nth term of the sequence (indexed from 1). */
seq_t a(idx_t n) {
    if (n < 1) return 0;                /* undefined for n < 1 */

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_used = 0;
    static size_t cache_cap  = 0;
    static idx_t  last_checked = 2;     /* next integer to test */

    /* Extend the cache until it contains at least n entries. */
    while (cache_used < (size_t)n) {
        idx_t cand = last_checked++;
        if (!is_prime(cand)) continue;

        idx_t v1 = 2 * cand - 3;       /* 2p-3 */
        idx_t v2 = 3 * cand - 2;       /* 3p-2 */
        if (!is_prime(v1) || !is_prime(v2)) continue;

        /* Store p only if it fits in the target type. */
        seq_t cand_seq = (seq_t)cand;
        if (cand_seq < cand) break;    /* overflow: further values will overflow too */

        ensure_capacity(cache_used + 1, &cache, &cache_cap);
        cache[cache_used++] = cand_seq;
    }

    /* If we did not find enough fitting terms, return 0. */
    if (cache_used < (size_t)n) return 0;
    return cache[n - 1];
}
