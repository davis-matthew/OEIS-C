/*
 * A274222  Numbers k = p_1^s_1...p_m^s_m such that k/p_i^s_i == 1 (mod p_i)
 *          for all 0 < i <= m.
 * https://oeis.org/A274222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 6768, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A274222: numbers k such that for each prime power p^s || k,
   the cofactor k/p^s ≡ 1 (mod p).  The function searches integers
   sequentially and keeps the terms in a cache. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/floor_sqrt.h"          /* seq_t floor_sqrt(seq_t x); */
#include "../utils/build_primes_upto.h"   /* void build_primes_upto(idx_t limit,
                                           uint64_t **primes_out, size_t *cnt_out); */

static void ensure_cache_capacity(size_t need, seq_t **ptr, size_t *cap)
{
    if (need > *cap) {
        size_t newcap = (*cap == 0) ? 1 : (*cap * 2);
        if (newcap < need) newcap = need;
        *ptr = (seq_t *)realloc(*ptr, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

static bool is_valid_candidate(seq_t x, const uint64_t *primes, size_t primes_cnt)
{
    if (x == 1) return true;            /* 1 always satisfies. */

    seq_t temp = x;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p64 = primes[i];
        seq_t p = (seq_t)p64;
        seq_t p_sq = p * p;
        if (p_sq > temp) break;

        if (temp % p == 0) {
            seq_t p_power = 1;
            while (temp % p == 0) {
                temp /= p;
                p_power *= p;
            }
            seq_t m = x / p_power;
            if (m % p != 1) return false;
        }
    }

    if (temp > 1) {                     /* remaining prime factor */
        seq_t p = temp;
        seq_t m = x / p;
        if (m % p != 1) return false;
    }

    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but safe guard */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static uint64_t primes_limit = 0;  /* maximum value stored in primes */

    static seq_t next_candidate = 2;

    /* initialise cache */
    if (cache == NULL) {
        ensure_cache_capacity(1, &cache, &cache_cap);
        cache[0] = (seq_t)1;   /* a(1) = 1 */
        cache_len = 1;
    }

    /* grow prime list to exceed sqrt(next_candidate) */
    if (primes == NULL || next_candidate > (seq_t)primes_limit * (seq_t)primes_limit) {
        uint64_t new_limit = primes_limit ? primes_limit * 2 : 1000;
        seq_t sqrt_candidate = floor_sqrt((seq_t)next_candidate);
        uint64_t sqrt_val = (uint64_t)sqrt_candidate;
        if (new_limit < sqrt_val * 2) new_limit = sqrt_val * 2;
        uint64_t *new_primes = NULL;
        size_t new_cnt = 0;
        build_primes_upto(new_limit, &new_primes, &new_cnt);
        free(primes);
        primes = new_primes;
        primes_cnt = new_cnt;
        primes_limit = new_limit;
    }

    while (cache_len < (size_t)n) {
        seq_t x = next_candidate;
        if (is_valid_candidate(x, primes, primes_cnt)) {
            ensure_cache_capacity(cache_len + 1, &cache, &cache_cap);
            cache[cache_len++] = x;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
