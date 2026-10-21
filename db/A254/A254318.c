/*
 * A254318  Hyper equidigital numbers.
 * https://oeis.org/A254318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for hyper equidigital numbers: test each integer for the
 * property (distinct digits in prime factorisation = distinct digits in
 * the number itself) and cache the results for future calls. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/digit_counts.h"

/* Helper: determine if a 64‑bit integer is a hyper equidigital number. */
static bool is_hyper_equidigital(uint64_t n)
{
    /* Factorisation storage (enough for ordinary 64‑bit numbers). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    unsigned char used[10] = {0};
    unsigned char tmp[10];

    /* Process prime parts. */
    for (size_t i = 0; i < cnt; ++i) {
        digit_counts((seq_t)primes[i], tmp);
        for (int d = 0; d < 10; ++d)
            if (tmp[d]) used[d] = 1;
    }

    /* Process exponents > 1. */
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] > 1) {
            digit_counts((seq_t)exps[i], tmp);
            for (int d = 0; d < 10; ++d)
                if (tmp[d]) used[d] = 1;
        }
    }

    /* Count distinct digits used in the factorisation. */
    int distinct = 0;
    for (int d = 0; d < 10; ++d)
        if (used[d]) ++distinct;

    /* Count distinct digits in the number itself. */
    digit_counts((seq_t)n, tmp);
    int distinct_n = 0;
    for (int d = 0; d < 10; ++d)
        if (tmp[d]) ++distinct_n;

    return distinct == distinct_n;
}

/* Static storage for memoisation. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static uint64_t next_candidate = 2;          /* first term is 2 (a(1)=2) */

/* Return the n‑th hyper equidigital number. */
seq_t a(idx_t n)
{
    /* Grow the cache until the n‑th term is present. */
    while (cache_len < (size_t)n) {
        /* Check the current candidate. */
        if (is_hyper_equidigital(next_candidate)) {
            /* Ensure enough room in the array. */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                /* Cast for C++ compatibility. */
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            /* Store the candidate (promoted to seq_t). */
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];         /* 1‑based indexing. */
}
