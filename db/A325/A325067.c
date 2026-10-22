/*
 * A325067  Prime numbers congruent to 1 modulo 16 representable by both x^2 +
 *          32*y^2 and x^2 + 64*y^2.
 * https://oeis.org/A325067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p ≡ 1 (mod 16) such that * p = x^2 + 32
 *            y^2 and p = u^2 + 64 v^2 for some integers x,y,u,v. * The
 *            function searches for these primes on demand and memoises the *
 *            results. It works for any seq_t (ordinary unsigned integer or *
 *            arbitrary‑precision class) and any idx_t (uint64_...
 * Range:     u8 n <= 1, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ----------  integer square root (uint64_t)  ---------- */
static inline uint64_t uint64_isqrt(uint64_t n)
{
    uint64_t res = 0;
    uint64_t bit = (uint64_t)1U << 62;          /* highest power of 4 ≤ n */
    while (bit > n) bit >>= 2;
    for (; bit; bit >>= 2)
    {
        uint64_t temp = res + bit;
        if (n >= temp)
        {
            n -= temp;
            res = (res >> 1) + bit;
        }
        else
            res >>= 1;
    }
    return res;
}

/* ----------  helper: check representation p = a^2 + k*b^2  ---------- */
static inline bool has_form(uint64_t p, uint64_t k)
{
    uint64_t max_y = uint64_isqrt(p / k);   /* y such that k*y^2 ≤ p */
    for (uint64_t y = 0; y <= max_y; ++y)
    {
        uint64_t yy  = y * y;
        uint64_t t   = p - k * yy;
        uint64_t s   = uint64_isqrt(t);
        if (s * s == t)
            return true;
    }
    return false;
}

/* ----------  helper: both forms  ---------- */
static inline bool has_both_forms(uint64_t p)
{
    return has_form(p, 32) && has_form(p, 64);
}

/* ----------  trial‑division prime test  ---------- */
static inline bool is_prime_candidate(uint64_t p,
                                      const uint64_t *primes,
                                      size_t count)
{
    if (p < 2) return false;
    uint64_t limit = uint64_isqrt(p);
    for (size_t i = 0; i < count && primes[i] <= limit; ++i)
        if (p % primes[i] == 0)
            return false;
    return true;
}

/* ----------  dynamic array helpers  ---------- */
static void grow_uint64(uint64_t **ptr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
    if (new_cap < needed) new_cap = needed;
    uint64_t *tmp = (uint64_t *)realloc(*ptr, new_cap * sizeof(uint64_t));
    if (tmp) {
        *ptr = tmp;
        *cap = new_cap;
    }
}

/* ----------  static state  ---------- */
static uint64_t *prime_list   = NULL;
static size_t    prime_cnt    = 0;
static size_t    prime_cap    = 0;

static uint64_t *prime_cache  = NULL;
static size_t    cache_cnt    = 0;
static size_t    cache_cap    = 0;

static uint64_t next_prime_candidate = 3;   /* start with first odd prime */

/* ----------  main sequence function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for indices < 1 */

    /* Ensure the cache contains at least n terms */
    while (cache_cnt < (size_t)n)
    {
        /* Generate primes incrementally */
        if (prime_cnt < 2) {           /* initialise prime list with 2 */
            grow_uint64(&prime_list, &prime_cap, 2);
            prime_list[prime_cnt++] = 2;
        }

        /* Test current candidate for primality */
        if (is_prime_candidate(next_prime_candidate, prime_list, prime_cnt))
        {
            /* Add to list of primes */
            grow_uint64(&prime_list, &prime_cap, prime_cnt + 1);
            prime_list[prime_cnt++] = next_prime_candidate;

            /* Interested only in primes p ≡ 1 (mod 16) */
            if ((next_prime_candidate & 15ULL) == 1ULL &&
                has_both_forms(next_prime_candidate))
            {
                /* Store in cache */
                grow_uint64(&prime_cache, &cache_cap, cache_cnt + 1);
                prime_cache[cache_cnt++] = next_prime_candidate;
            }
        }

        /* Advance to next odd candidate */
        if (next_prime_candidate == 3)
            next_prime_candidate = 5;
        else
            next_prime_candidate += 2;
    }

    return (seq_t)prime_cache[(size_t)n - 1];
}
