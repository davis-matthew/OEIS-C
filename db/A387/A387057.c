/*
 * A387057  Numbers k that are infinitarily divisible by the number of
 *          infinitary divisors of k.
 * https://oeis.org/A387057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 4637, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A387057: numbers k such that A037445(k) is an infinitary divisor of k.
   A037445(k) = 2^{sum popcount(exponents of primes in k)}.  Then 2^{S} is
   an infinitary divisor of k iff k contains the prime 2 with exponent e2
   that has all bits of S as a submask. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static const uint32_t SIEVE_LIMIT = 2000000;

/* Smallest prime factor array.  Index i holds the smallest prime that
   divides i.  0 and 1 are left 0. */
static uint32_t *spf = NULL;
static uint32_t sieve_done = 0;

/* Sequence cache. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Current integer being examined. */
static uint32_t current_k = 1;

/* Initialize the sieve of smallest prime factors. */
static void init_sieve(void)
{
    if (sieve_done) return;

    spf = (uint32_t *)malloc((SIEVE_LIMIT + 1) * sizeof(uint32_t));
    for (uint32_t i = 0; i <= SIEVE_LIMIT; ++i) spf[i] = i;

    for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i)
        if (spf[i] == i)
            for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                if (spf[j] == j) spf[j] = i;

    cache_cap = 128;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    sieve_done = 1;
}

/* Test if k is a term of the sequence. */
static bool k_is_term(uint32_t k)
{
    if (k == 1) return true;           /* 1 is a term */

    uint32_t n = k;
    uint32_t S = 0;                     /* sum of popcounts of exponents */
    uint32_t e2 = 0;                    /* exponent of prime 2 */

    while (n > 1) {
        uint32_t p = spf[n];
        uint32_t e = 0;
        while (n > 1 && spf[n] == p) {
            n /= p;
            ++e;
        }
        S += __builtin_popcount(e);
        if (p == 2) e2 = e;
    }

    if (e2 == 0) return false;          /* no factor 2, cannot contain 2^S */

    /* Submask condition: all bits of S must be present in e2. */
    return (S & ~e2) == 0;
}

/* Ensure that the cache contains at least n terms. */
static void ensure_terms(idx_t n)
{
    while (cache_len < n) {
        if (current_k > SIEVE_LIMIT) {
            /* Should not happen for requested range.  If it does,
               extend sieve (not implemented) and exit. */
            return;
        }
        if (k_is_term(current_k)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)current_k;
        }
        ++current_k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for index 0 */

    init_sieve();
    ensure_terms(n);
    return cache[n - 1];
}
