/*
 * A334802  Positive integers of the form x^4 - y^4 that have exactly 4
 *          divisors.
 * https://oeis.org/A334802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (b(n)+1)^4 - b(n)^4 where b(n) is the n‑th integer x such
 *            that 2x-1 and 2x^2-2x+1 are prime. The function generates the
 *            numbers x^4-(x-1)^4 for successive x meeting these primality
 *            conditions
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 53, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul64.h"

/* ---------- deterministic Miller–Rabin for 64‑bit ---------- */

static uint64_t mod_pow_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    a %= mod;
    while (e)
    {
        if (e & 1) r = mul64(r, a, mod);
        a = mul64(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool is_prime_64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes first */
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    /* write n-1 = d * 2^s with d odd */
    uint64_t d = n - 1;
    unsigned int s = 0;
    while ((d & 1ULL) == 0) { d >>= 1; ++s; }
    /* bases sufficient for deterministic test under 2^64 */
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned int r = 1; r < s; ++r)
        {
            x = mul64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- sequence generation ---------- */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t next_x = 2; /* next x to test, starting at 2 */

    /* extend cache until we have n terms */
    while (cache_len < (size_t)n)
    {
        uint64_t x = next_x++;
        uint64_t p = 2 * x - 1;
        unsigned __int128 xx = (unsigned __int128)x;
        unsigned __int128 q128 = 2 * xx * xx - 2 * xx + 1;
        uint64_t q = (uint64_t)q128;

        if (!is_prime_64(p) || !is_prime_64(q))
            continue; /* not a valid term */

        /* compute a = x^4 - (x-1)^4 = 4x^3 - 6x^2 + 4x - 1 */
        seq_t xseq = (seq_t)x;
        seq_t val = 4 * xseq * xseq * xseq
                    - 6 * xseq * xseq
                    + 4 * xseq
                    - 1;

        /* store in cache */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort(); /* OOM */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = val;
    }

    return cache[(size_t)n - 1];
}
