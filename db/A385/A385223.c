/*
 * A385223  Primes p such that multiplicative order of -3 modulo p is odd.
 * https://oeis.org/A385223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 2169, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small, mod_pow, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that the multiplicative order of -3 modulo p is odd */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"
#include "../utils/mod_pow.h"
#include "../utils/seq_realloc.h"

/* test whether a prime p satisfies the condition */
static bool is_good_prime(uint64_t p)
{
    if (p == 2) return true;          /* 2 is the first term */
    if (p == 3) return false;         /* -3 ≡ 0 (mod 3), excluded */

    seq_t base = (seq_t)(p - 3);      /* -3 modulo p */

    /* factor p‑1 to obtain its distinct prime factors */
    uint64_t t    = p - 1;
    uint64_t tmp  = t;
    uint64_t factors[64];
    size_t   fcnt = 0;
    for (uint64_t d = 2; d * d <= tmp; ++d) {
        if (tmp % d == 0) {
            factors[fcnt++] = d;
            while (tmp % d == 0) tmp /= d;
        }
    }
    if (tmp > 1) factors[fcnt++] = tmp;

    /* compute the multiplicative order of `base` modulo p */
    uint64_t ord = t;
    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t q = factors[i];
        while (ord % q == 0) {
            uint64_t cand = ord / q;
            seq_t r = mod_pow(base, cand, (seq_t)p);
            if (r != (seq_t)1) break;
            ord = cand;
        }
    }
    return (ord & 1ULL) != 0;         /* order must be odd */
}

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next     = 2;    /* next candidate to test */

    /* grow cache until it contains at least n elements */
    while (cache_len < (size_t)n) {
        /* find next prime */
        uint64_t p = next;
        while (!is_prime_small((seq_t)p)) p++;

        next = p + 1;                /* start search after this prime */

        if (is_good_prime(p)) {
            /* enlarge cache if necessary */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) exit(1);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)p;
        }
    }
    return cache[n - 1];
}
