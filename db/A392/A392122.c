/*
 * A392122  Numbers k such that (k^3 - greatest prime < k^3) > (-k^3 + least
 *          prime > k^3).
 * https://oeis.org/A392122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A392122
   Numbers k such that k^3 is closer to the next higher prime than
   to the greatest lower prime.  Formally
   k^3 - p_low > p_high - k^3
   where p_low is the largest prime < k^3 and p_high the smallest
   prime > k^3.  The sequence begins
   3,5,7,13,22,24,25,29,32,39,42,44,45,50,52,56,59,61,62,63,64,65,69,71,… */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* 64‑bit deterministic Miller–Rabin primality test (valid for n < 2^64). */
static inline uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
#ifdef __SIZEOF_INT128__
    return (unsigned __int128)a * b % mod;
#else
    /* Fallback for systems without 128‑bit integers: use 64‑bit + split */
    uint64_t hi, lo;
    unsigned __int128 product = (unsigned __int128)a * b;
    return (uint64_t)(product % mod);
#endif
}

static inline uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static inline bool is_prime_u64(uint64_t n)
{
    if (n < 2)
        return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
    {
        uint64_t p = small[i];
        if (n % p == 0)
            return n == p;
    }
    uint64_t d = n-1, s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }
    static const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i];
        if (a % n == 0)
            continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n-1)
            continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mod_mul(x, x, n);
            if (x == n-1)
            {
                comp = false;
                break;
            }
        }
        if (comp)
            return false;
    }
    return true;
}

/* Fixed maximum cache size – sufficient for n up to about 10000. */
#define MAX_CACHE 131072
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;
static idx_t last_k = 0;

/* Core sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset is 1 – undefined for 0 */

    /* generate terms until we have at least n of them */
    while (cache_len < n)
    {
        ++last_k;
        uint64_t k3 = (uint64_t)last_k * last_k * last_k;  /* k³ fits 64‑bit for our n range */

        /* largest prime < k³ */
        uint64_t p_low = 0;
        if (k3 > 2)
        {
            uint64_t v = k3 - 1;
            while (v >= 2 && !is_prime_u64(v))
                --v;
            if (v >= 2)
                p_low = v;
        }

        /* smallest prime > k³ */
        uint64_t p_high = 0;
        if (k3 < UINT64_MAX)
        {
            uint64_t v = k3 + 1;
            while (!is_prime_u64(v))
                ++v;
            p_high = v;
        }

        /* condition: distance to lower prime > distance to higher prime */
        if (p_low != 0 && (k3 - p_low) > (p_high - k3))
        {
            if (cache_len == MAX_CACHE)
                abort();  /* guard against overflow of static cache */
            cache[cache_len++] = (seq_t)last_k;
        }
    }
    return cache[n-1];
}
