/*
 * A348929  a(n) = gcd(n, A003959(n)), where A003959 is multiplicative with
 *          a(p^e) = (p+1)^e.
 * https://oeis.org/A348929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = gcd( n , Π (p+1)^e ) where n = Π p^e (A003959)
 * Range:     u8 n <= 47, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9303 (2026-11-03)
 * Helpers:   gcd, gcd_u64, ipow, is_prime_u64, mem, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/gcd_u64.h"
#include "../utils/ipow.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

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

seq_t a(idx_t n)
{
    /* Offset 1: a(1) is the first term */
    if (n == 0)
        return 0;        /* undefined for n==0 */

    /* static cache: cache[i] holds a(i) once computed; 0 indicates "not yet" */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (cache == NULL || n >= cache_cap)
        ensure_cache(n, &cache, &cache_cap);

    if (cache[n] != 0)
        return cache[n];

    /* Compute A003959(n) = Π (p+1)^e over prime powers of n */
    seq_t prod = 1;
    uint64_t temp = (uint64_t)n;

    while (temp > 1) {
        uint64_t p = smallest_prime_factor(temp);
        uint64_t e = 0;
        do {
            temp /= p;
            ++e;
        } while (temp % p == 0);

        seq_t base = (seq_t)(p + 1);
        seq_t pk   = ipow(base, (seq_t)e);
        prod *= pk;
    }

    seq_t res = gcd(prod, (seq_t)n);

    cache[n] = res;
    return res;
}
