/*
 * A272596  Numbers n such that the multiplicative group modulo n is the
 *          direct product of 6 cyclic groups.
 * https://oeis.org/A272596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer n (with n ≥ 1) satisfying A046072(n) =
 *            6; i.e. n has 4 distinct odd primes with the exponent of 2 at
 *            least 3, OR 5 distinct odd primes with exponent of 2 equal 2, OR
 *            6 distinct odd primes with exponent of 2 0 or 1
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Maximum value up to which the smallest prime factor table is built.
   20 000 000 covers over ten thousand terms for all tested builds. */
#define MAX_LIMIT 20000000ULL

static uint32_t *spf = NULL;          /* smallest prime factor array   */
static size_t  spf_limit = 0;        /* current table size             */

/* Build smallest‑prime‑factor table up to 'limit'. */
static void build_spf(size_t limit)
{
    spf_limit = limit;
    spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!spf) exit(1);               /* out of memory: abort            */

    for (size_t i = 0; i <= limit; ++i) spf[i] = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = (uint32_t)i;
            if ((unsigned long long)i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i) {
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
                }
            }
        }
    }
    spf[1] = 1;
}

/* Return number of trailing zeros in binary representation of v. */
static inline unsigned trailing_zeros(idx_t v)
{
    unsigned c = 0;
    while ((v & 1ULL) == 0ULL) {
        ++c;
        v >>= 1;
    }
    return c;
}

/* Sequence cache */
static seq_t *cache      = NULL;      /* array holding found terms      */
static size_t cache_len  = 0;          /* number of terms stored         */
static size_t cache_cap  = 0;          /* current allocation capacity    */
static idx_t  last_checked = 1;        /* last number examined so far    */
static bool   initialized = false;     /* whether table and cache exist  */

static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* Build the SPF table once. */
        build_spf(MAX_LIMIT);
        /* First cache entry will be filled inside the loop below. */
        initialized = true;
    }

    while (cache_len < (size_t)n) {
        ++last_checked;                      /* next integer to test   */
        /* Ensure last_checked is within the SPF table. */
        if ((size_t)last_checked > spf_limit) {
            /* Should not happen for the limits used. */
            return 0;
        }

        /* Determine exponent e of 2 in last_checked. */
        unsigned e = trailing_zeros(last_checked);
        unsigned f;                          /* contribution to count */
        if (e <= 1) f = 0;
        else if (e == 2) f = 1;
        else f = 2;

        /* Remove powers of two to obtain odd part p. */
        idx_t p = last_checked >> e;

        /* Count distinct odd prime factors of p. */
        int odd_primes = 0;
        while (p > 1) {
            uint32_t prime = spf[p];
            ++odd_primes;
            while (p % prime == 0) p /= prime;
        }

        if (odd_primes + f == 6) {
            /* Store the qualifying term. */
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len] = (seq_t)last_checked;
            ++cache_len;
        }
    }

    return cache[n - 1];
}
