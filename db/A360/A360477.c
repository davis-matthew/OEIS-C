/*
 * A360477  Numbers whose product of distinct prime factors is greater than or
 *          equal to the sum of its prime factors (with repetition).
 * https://oeis.org/A360477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers k where product of distinct prime factors of k (rad(k))
 *            is greater than or equal to sum of prime factors of k counted
 *            with multiplicity. The sequence (OEIS A360477) is indexed with
 *            offset 1
 * Range:     u8 n <= 214, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static inline bool satisfies(uint64_t m) {
    if (m == 1) return true;          /* 1 is a term */
    uint64_t n = m;
    uint64_t sum = 0;
    uint64_t prod = 1;
    for (uint64_t p = 2; p <= n / p; ) {
        if (n % p == 0) {
            bool first = true;
            while (n % p == 0) {
                sum += p;
                n /= p;
                if (first) {
                    prod *= p;
                    first = false;
                }
            }
        }
        if (p == 2) p = 3;
        else p += 2;                   /* skip even numbers after 2 */
    }
    if (n > 1) {                       /* remaining prime factor */
        sum += n;
        prod *= n;
    }
    return prod >= sum;
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;
static uint64_t next_candidate = 2;
static bool cache_initialized = false;

static void init_cache(void) {
    if (cache_initialized) return;
    cache_cap = 64;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(1);              /* memory failure */
    cache[0] = (seq_t)1;              /* first term */
    cache_len = 1;
    next_candidate = 2;
    cache_initialized = true;
}

seq_t a(idx_t n) {
    init_cache();
    while (cache_len < n) {
        uint64_t m = next_candidate;
        while (!satisfies(m)) ++m;
        next_candidate = m + 1;
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache) exit(1);
        }
        cache[cache_len++] = (seq_t)m;
    }
    return cache[(size_t)(n - 1)];
}
