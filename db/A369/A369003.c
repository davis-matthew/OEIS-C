/*
 * A369003  Numbers k for which k' / gcd(k,k') is odd, where k' stands for the
 *          arithmetic derivative of k, A003415.
 * https://oeis.org/A369003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 175, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k for which k' / gcd(k,k') is odd, where k' is the arithmetic
 * derivative (OEIS A003415).  The sequence is generated lazily and
 * memoised, so any index n ≥ 1 is answered correctly regardless of
 * call order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static bool passes(uint64_t k)
{
    /* Arithmetic derivative: d(p^e) = e * p^(e-1). */
    if (k < 2) return false;

    seq_t n_val = (seq_t)k;
    seq_t deriv = 0;
    uint64_t tmp = k;

    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            deriv += (n_val / (seq_t)p) * (seq_t)e;
        }
    }

    if (tmp > 1) {                     /* remaining prime factor */
        deriv += (n_val / (seq_t)tmp); /* exponent = 1 */
    }

    seq_t g = gcd(n_val, deriv);
    seq_t q = deriv / g;
    return (q & (seq_t)1) != 0;        /* odd? */
}

/* Simple cache of accepted numbers. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t next_candidate = 2;

/* Make sure the cache holds at least n values. */
static void ensure_cache(idx_t n)
{
    while (cache_len < (size_t)n) {
        if (passes(next_candidate)) {
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
