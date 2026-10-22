/*
 * A385416  The number of unordered factorizations of n into exponentially odd
 *          numbers (A268335).
 * https://oeis.org/A385416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n into
 *            exponentially odd numbers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* small hash map for memoisation */
#define MAP_SIZE 200003  /* a prime number for better distribution */

struct CacheEntry {
    uint64_t key;
    seq_t   val;
    bool    used;
};

static struct CacheEntry cache[MAP_SIZE] = {0};

static inline size_t hash64(uint64_t x) {
    /* simple mix */
    x ^= x >> 33;
    x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33;
    x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;
    return (size_t)x;
}

static inline seq_t *cache_find(uint64_t key) {
    size_t h = hash64(key) % MAP_SIZE;
    while (cache[h].used) {
        if (cache[h].key == key) return &cache[h].val;
        h = (h + 1) % MAP_SIZE;
    }
    return NULL;
}

static inline void cache_insert(uint64_t key, seq_t val) {
    size_t h = hash64(key) % MAP_SIZE;
    while (cache[h].used) h = (h + 1) % MAP_SIZE;
    cache[h].used = true;
    cache[h].key  = key;
    cache[h].val  = val;
}

/* forward declaration */
static seq_t count_factorizations(uint64_t n, uint64_t min_factor);

/* recursive enumeration of exponentially odd divisors */
static void explore(uint64_t idx,
                    uint64_t current,
                    const uint64_t *primes,
                    const uint64_t *exps,
                    size_t           cnt,
                    uint64_t         min_factor,
                    uint64_t         original_n,
                    seq_t            *sum)
{
    if (idx == cnt) {
        if (current > 1 && current >= min_factor) {
            seq_t sub = count_factorizations(original_n / current, current);
            *sum += sub;
        }
        return;
    }

    /* exponent 0 */
    explore(idx + 1, current, primes, exps, cnt, min_factor, original_n, sum);

    /* odd exponents > 0 */
    uint64_t val = current;
    for (uint64_t e = 1; e <= exps[idx]; ++e) {
        val *= primes[idx];
        if (e & 1U) { /* odd exponent */
            explore(idx + 1, val, primes, exps, cnt, min_factor, original_n, sum);
        }
    }
}

/* memoised count function */
static seq_t count_factorizations(uint64_t n, uint64_t min_factor) {
    if (n == 1) return 1;  /* empty factorization */

    uint64_t key = (n << 32) | min_factor;
    seq_t *cached = cache_find(key);
    if (cached) return *cached;

    seq_t res = 0;
    /* factor n */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t sum = 0;
    explore(0, 1, primes, exps, cnt, min_factor, n, &sum);
    res = sum;

    cache_insert(key, res);
    return res;
}

/* public entry function */
seq_t a(idx_t n) {
    return count_factorizations((uint64_t)n, 2);
}
