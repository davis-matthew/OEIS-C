/*
 * A383907  Echo primes: primes p such that the greatest prime factor of p-1
 *          is a suffix of p.
 * https://oeis.org/A383907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   digits_of, largest_prime_factor, next_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Echo primes:  a prime p such that the greatest prime factor of p-1
    is a suffix of p.  The function searches primes in increasing order,
    keeps a static cache of found terms, and returns the n-th term
    (a(1) is the first term, 13).  The cache is memoised across calls. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/next_prime.h"
#include "../utils/largest_prime_factor.h"
#include "../utils/pow10.h"
#include "../utils/digits_of.h"

/* static cache for the sequence values */
static seq_t *cache_vals = NULL;
static size_t cache_sz   = 0;     /* number of values stored */
static size_t cache_cap  = 0;     /* allocated capacity */
static uint64_t last_checked = 0; /* last prime examined */
static bool cache_inited = false;

/* grow the cache array as needed */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ?: 64;
    while (new_cap < needed) new_cap *= 2;
    cache_vals = (seq_t *)realloc(cache_vals, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* ensure the cache contains at least n terms */
static void ensure_cache(idx_t n)
{
    if (!cache_inited) {
        last_checked = 2;           /* so next_prime(3) is the first prime */
        cache_inited = true;
    }
    while (cache_sz < (size_t)n) {
        uint64_t p = next_prime(last_checked + 1);
        last_checked = p;                        /* remember last prime seen */

        /* greatest prime factor of p-1 */
        uint64_t g = largest_prime_factor(p - 1);

        /* compute 10^digits(g) */
        idx_t exp = digits_of(g);
        seq_t pow10v = pow10(exp);

        /* suffix of p with length exp */
        seq_t suffix = (seq_t)p % pow10v;

        if (suffix == (seq_t)g) {
            grow_cache(cache_sz + 1);
            cache_vals[cache_sz++] = (seq_t)p;
        }
    }
}

/* public function: return the n-th echo prime (n>=1) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* offset is 1, return 0 for invalid indices */
    ensure_cache(n);
    return cache_vals[n - 1];
}
