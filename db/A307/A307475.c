/*
 * A307475  SanD-86 primes: primes p such that p+d is also prime and sum of
 *          digits A007953(p(p+d)) = d, with d = 86.
 * https://oeis.org/A307475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   digit_sum, is_prime_big, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th SanD‑86 prime – a prime p such that
 *   1) p is prime,
 *   2) p+86 is prime,
 *   3) digit_sum(p * (p+86)) == 86.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/digit_sum.h"
#include "../utils/next_prime.h"

#ifdef __cplusplus
#include <vector>
/* C++ implementation – uses std::vector for storage */
static std::vector<seq_t> cache;
static uint64_t next_candidate = 3;

seq_t a(idx_t n) {
    while (cache.size() < (size_t)n) {
        uint64_t p = next_candidate;
        p = next_prime(p);              /* smallest prime >= p */
        next_candidate = p + 1;         /* advance for next search */

        uint64_t q = p + 86;
        if (!is_prime_big((seq_t)q))    /* p+86 must be prime */
            continue;

        seq_t prod = (seq_t)p * (seq_t)q;           /* p * (p+86) */
        if (digit_sum(prod) != (seq_t)86)
            continue;

        cache.push_back((seq_t)p);          /* record SanD‑86 prime */
    }
    return cache[(size_t)n - 1];            /* 1‑based indexing */
}
#else
/* C implementation – uses malloc/realloc for storage */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 3;

static void ensure_capacity(size_t needed) {
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failure – terminate */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    while (cache_len < (size_t)n) {
        uint64_t p = next_candidate;
        p = next_prime(p);              /* smallest prime >= p */
        next_candidate = p + 1;         /* advance for next search */

        uint64_t q = p + 86;
        if (!is_prime_big((seq_t)q))    /* p+86 must be prime */
            continue;

        seq_t prod = (seq_t)p * (seq_t)q;           /* p * (p+86) */
        if (digit_sum(prod) != (seq_t)86)
            continue;

        ensure_capacity(cache_len + 1);
        cache[cache_len++] = (seq_t)p;             /* record SanD‑86 prime */
    }
    return cache[(size_t)n - 1];
}
#endif
