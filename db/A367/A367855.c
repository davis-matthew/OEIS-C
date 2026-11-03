/*
 * A367855  The slowest increasing sequence of semiprimes such that a(n-1) +
 *          a(n) is prime.
 * https://oeis.org/A367855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 980, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9908 (2026-11-03)
 * Helpers:   gcd_u64, is_prime, is_prime_u64, mem, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utility functions supplied by the project */
#include <string.h>
#include "../utils/gcd_u64.h"
#include "../utils/is_prime.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/mem.h"

/* Pollard–Rho: find a non‑trivial divisor of n (n composite) */
static uint64_t pollard(uint64_t n) {
    if (n % 2 == 0) return 2;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
        }
        if (d != n) return d;
        ++c;  /* try another constant */
    }
}

/* recursive smallest prime factor */
static uint64_t smallest_prime_factor(uint64_t n) {
    if (n % 2 == 0) return 2;
    if (is_prime_u64(n)) return n;
    uint64_t d = pollard(n);
    uint64_t s1 = smallest_prime_factor(d);
    uint64_t s2 = smallest_prime_factor(n / d);
    return (s1 < s2) ? s1 : s2;
}

/* This function finds the n‑th term of OEIS sequence A367855.
   It searches for the smallest semiprime greater than the
   previous term that together with that term sums to a prime. */
static bool is_semiprime(uint64_t n)
{
    if (n < 4) return false;                     /* 4 is the smallest semiprime */
    uint64_t spf = smallest_prime_factor(n);    /* smallest prime factor */
    if (spf == n) return false;                  /* n is prime */
    uint64_t q = n / spf;
    return is_prime((idx_t)spf) && is_prime((idx_t)q);
}

static seq_t *cache       = NULL;  /* cache[0] holds a(1) */
static size_t cache_len   = 0;      /* number of values stored so far */
static size_t cache_cap   = 0;      /* allocated capacity */

/* Grow the cache so that it can store at least `min_cap` elements. */
static void grow_cache(size_t min_cap)
{
    if (min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < min_cap) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();                /* allocation failure */
    cache      = tmp;
    cache_cap  = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n < 1 */

    size_t idx = (size_t)n;           /* index into cache (0‑based) */

    /* Initialise cache with the first term if empty. */
    if (cache_len == 0) {
        grow_cache(1);
        cache[0] = (seq_t)4;           /* a(1) = 4 */
        cache_len = 1;
    }

    /* Generate terms up to index `idx-1` if needed. */
    if (idx > cache_len) {
        grow_cache(idx);
        for (size_t cur = cache_len; cur < idx; ++cur) {
            uint64_t prev = (uint64_t)cache[cur - 1];
            uint64_t candidate = prev + 1;
            while (true) {
                if (is_semiprime(candidate)) {
                    uint64_t sum = prev + candidate;
                    if (is_prime((idx_t)sum)) break;
                }
                ++candidate;
            }
            cache[cur] = (seq_t)candidate;
        }
        cache_len = idx;
    }

    return cache[idx - 1];
}
