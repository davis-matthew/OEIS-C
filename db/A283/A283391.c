/*
 * A283391  Primes p such that a^2 + b^2 = p^2 and q = a + b is prime, where
 *          a,b > 0.
 * https://oeis.org/A283391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1231, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A283391.
   For each prime p ≡ 1 (mod 4), find the unique representation
   p = x² + y² with x > y > 0, then compute q = x² − y² + 2xy.
   If q is prime, store p. Return the nth such prime. */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Integer square root for 64‑bit values. */
static uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t x = n >> 1;
    uint64_t y = (x + n / x) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* Cache for the discovered sequence terms. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static uint64_t next_candidate = 5;  /* first prime ≡ 1 mod 4 */

/* Ensure the cache can hold 'need' elements. */
static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 32;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* allocation failed */
    cache      = tmp;
    cache_cap  = new_cap;
}

/* Generate terms until we have at least 'n' of them. */
static void build_up_to(idx_t n)
{
    if (cache_len >= (size_t)n) return;

    while (cache_len < (size_t)n) {
        /* Find the next prime of the form 4k+1. */
        while (!is_prime((idx_t)next_candidate))
            next_candidate += 4;

        uint64_t p = next_candidate;

        /* Find the unique representation p = x² + y². */
        uint64_t sqrt_p = isqrt_u64(p);
        uint64_t x = 0, y = 0;
        for (x = sqrt_p; x > 0; --x) {
            uint64_t y2 = p - x * x;
            if (y2 == 0) continue; /* y must be > 0 */
            uint64_t yc = isqrt_u64(y2);
            if (yc * yc == y2 && yc > 0) {
                y = yc;
                break;
            }
        }

        if (x > y && y > 0) {
            uint64_t q = x * x - y * y + 2 * x * y;
            if (is_prime((idx_t)q)) {
                ensure_cache(cache_len + 1);
                cache[cache_len++] = (seq_t)p;
            }
        }

        next_candidate += 4;  /* advance to the next 1 mod 4 candidate */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined; harmless for OEIS offset 1 */
    build_up_to(n);
    return cache[n - 1];
}
