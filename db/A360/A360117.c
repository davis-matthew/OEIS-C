/*
 * A360117  Numbers k such that for all their prime factors p, v(p-1) <=
 *          v(k-1), where v(n) = A007814(n) is the 2-adic valuation of n.
 * https://oeis.org/A360117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A360117:
   Numbers k such that for all prime factors p of k,
   v(p-1) <= v(k-1), where v(n) is the 2‑adic valuation of n.
   The implementation builds the sequence by incremental sieving
   and caches the qualifying values.  This avoids harder searches
   while yielding all terms for any n≥1. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/*  Static data for caching sequence values and small primes           */
/* ------------------------------------------------------------------ */
static seq_t   *cache      = NULL;   /* sequence values            */
static size_t  cache_len   = 0;      /* number of values cached    */
static size_t  cache_cap   = 0;      /* capacity of cache array    */

static uint64_t last_checked = 1;     /* largest k tested so far    */

static uint32_t *primes     = NULL;   /* small prime list           */
static size_t   primes_len  = 0;      /* number of primes stored    */
static size_t   primes_cap  = 0;      /* capacity of primes array   */
static uint32_t max_prime_gen = 1;    /* largest prime generated    */

/* ------------------------------------------------------------------ */
/*  Helper: 2‑adic valuation for 64‑bit integers                      */
/* ------------------------------------------------------------------ */
static unsigned int v2_uint64(uint64_t x)
{
    unsigned int n = 0;
    while ((x & 1ULL) == 0ULL)
    {
        x >>= 1;
        ++n;
    }
    return n;
}

/* ------------------------------------------------------------------ */
/*  Ensure that the array of primes contains all primes ≤ limit        */
/* ------------------------------------------------------------------ */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= (uint64_t)max_prime_gen)
        return;

    /* Grow the prime array when necessary */
    if (primes_cap < limit + 16)
    {
        size_t new_cap = primes_cap == 0 ? 16 : primes_cap;
        while (new_cap < limit + 16)
            new_cap <<= 1;
        uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        primes = tmp;
        primes_cap = new_cap;
    }

    /* Generate further primes up to 'limit'                     */
    for (uint32_t n = max_prime_gen + 1; n <= (uint32_t)limit; ++n)
    {
        bool isprime = true;
        for (size_t i = 0; i < primes_len; ++i)
        {
            if ((uint64_t)primes[i] * primes[i] > n)
                break;
            if (n % primes[i] == 0)
            {
                isprime = false;
                break;
            }
        }
        if (isprime)
            primes[primes_len++] = n;
    }
    max_prime_gen = (uint32_t)limit;
}

/* ------------------------------------------------------------------ */
/*  Ensure the cache array can hold at least 'need' elements           */
/* ------------------------------------------------------------------ */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Test whether a given integer k satisfies the defining condition   */
/* ------------------------------------------------------------------ */
static bool satisfies_property(uint64_t k)
{
    if (k < 2)
        return false;

    unsigned int ek = v2_uint64(k - 1);   /* v(k-1) */

    uint64_t n = k;
    uint64_t root = (uint64_t)floor(sqrt((double)n));
    ensure_primes_upto(root);

    for (size_t i = 0; i < primes_len && (uint64_t)primes[i] * primes[i] <= n; ++i)
    {
        uint32_t p = primes[i];
        if (n % p == 0)
        {
            if (p != 2)
            {
                unsigned int vp = v2_uint64((uint64_t)p - 1);
                if (vp > ek)
                    return false;
            }
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1 && n != 2)   /* remaining prime factor */
    {
        unsigned int vp = v2_uint64(n - 1);
        if (vp > ek)
            return false;
    }

    return true;
}

/* ------------------------------------------------------------------ */
/*  Public function returning the n-th element of A360117              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* n is ≥ 1 by problem specification */
    while (cache_len < (size_t)n)
    {
        ++last_checked;
        if (satisfies_property(last_checked))
        {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
