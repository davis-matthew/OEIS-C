/*
 * A329731  Numbers k such that the sum of distinct prime divisors of k
 *          divides the sum of unitary divisors of k.
 * https://oeis.org/A329731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer k such that * the sum of
 *            distinct prime divisors of k divides the sum of * unitary
 *            divisors of k. * The function searches for such k and memoizes
 *            results in a cache
 * Range:     u8 n <= 33, u16 n <= 2126, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static seq_t *seq_cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 1;

/* Test whether k satisfies the condition.
 * Uses only 128‑bit arithmetic to avoid overflow in 8‑bit and 16‑bit builds. */
static bool is_good_uint64(uint64_t k)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(k, primes, exps, &pcnt);

    uint64_t sum_distinct = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        sum_distinct += primes[i];
    }
    if (sum_distinct == 0) return false;

    unsigned __int128 prod = 1;          /* product of (1 + p^e) modulo sum_distinct */
    for (size_t i = 0; i < pcnt; ++i) {
        unsigned __int128 base = primes[i];
        unsigned __int128 pow = 1;
        for (uint64_t e = exps[i]; e > 0; --e) {
            pow *= base;
        }
        unsigned __int128 unit = 1 + pow;
        prod = (prod * unit) % sum_distinct;
    }
    return prod == 0;
}

/* Grow the cache until it contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_size < n) {
        /* Find next qualifying number */
        while (!is_good_uint64(next_candidate))
            ++next_candidate;

        /* Grow storage if needed */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);          /* allocation failure */
            seq_cache = tmp;
            cache_cap = new_cap;
        }

        seq_cache[cache_size++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1; behaviour for 0 is undefined */
    ensure_cache(n);
    return seq_cache[n - 1];
}
