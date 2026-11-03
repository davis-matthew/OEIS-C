/*
 * A360979  Primes that share no digits with their digit sum.
 * https://oeis.org/A360979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   primes that share no digits with their digit sum
 * Range:     u8 n <= 34, u16 n <= 2151, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9784 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper: compute digit mask of x and its digit sum */
static uint16_t get_mask_and_sum(uint64_t x, uint16_t *mask, uint64_t *sum)
{
    uint16_t m = 0;
    uint64_t s = 0;
    if (x == 0) {
        m = 1;            /* digit 0 */
    } else {
        while (x) {
            uint64_t d = x % 10;
            m |= 1U << (unsigned)d;
            s += d;
            x /= 10;
        }
    }
    if (mask) *mask = m;
    if (sum)  *sum = s;
    return m;
}

/* Helper: true if prime p shares no digits with its digit sum */
static inline bool good_prime(uint64_t p)
{
    uint16_t mask_p, mask_sum;
    uint64_t sum;
    get_mask_and_sum(p, &mask_p, &sum);
    get_mask_and_sum(sum, &mask_sum, NULL);
    return (mask_p & mask_sum) == 0;
}

/* Ensure *ptr has capacity at least needed, reallocating if necessary */
static void ensure_capacity_seq(seq_t **ptr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        if (new_cap < needed) new_cap = needed;
        *ptr = mem_resize(seq_t, *ptr, new_cap);
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t *primes = NULL;
    static size_t primes_len = 0, primes_cap = 0;
    static uint64_t next_candidate = 2;
    static bool initialized = false;

    if (!initialized) {
        initialized = true;
        cache = mem_new(seq_t, 8);
        cache_cap = 8;
        primes = (uint64_t *)malloc(8 * sizeof(uint64_t));
        primes_cap = 8;
        primes_len = 0;
        cache_len = 0;
        next_candidate = 2;
    }

    /* Generate primes until we have at least n terms satisfying the property */
    while (cache_len < (size_t)n) {
        /* Find next prime */
        while (true) {
            bool is_prime = true;
            uint64_t m = next_candidate;
            if (m < 2) {
                is_prime = false;
            } else {
                /* Trial division by already found primes */
                for (size_t i = 0; i < primes_len; ++i) {
                    uint64_t p = primes[i];
                    if (p * p > m) break;
                    if (m % p == 0) {
                        is_prime = false;
                        break;
                    }
                }
            }
            if (is_prime) {
                /* Store prime for future trial divisions */
                if (primes_len + 1 > primes_cap) {   /* uint64_t, not seq_t (it was grown as seq_t) */
                    primes_cap *= 2;
                    uint64_t *t = (uint64_t *)realloc(primes, primes_cap * sizeof *t);
                    if (!t) abort();
                    primes = t;
                }
                primes[primes_len++] = m;

                /* Check the digit‑sum property */
                if (good_prime(m)) {
                    ensure_capacity_seq(&cache, &cache_cap, cache_len + 1);
                    cache[cache_len++] = (seq_t)m;
                }
            }
            ++next_candidate;
            if (cache_len >= (size_t)n) break;
        }
    }

    return cache[(size_t)n - 1];
}
