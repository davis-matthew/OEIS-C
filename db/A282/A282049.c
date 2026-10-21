/*
 * A282049  Least prime p such that n divides sigma(p*n).
 * https://oeis.org/A282049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that n divides σ(p · n)
 * Range:     u8 n <= 9, u16 n <= 598, u32 n <= 9191, u64 n <= 9191,
 *            u128 n <= 9191, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"

/* ------------------------------------------------------------------ */
/* Deterministic Miller–Rabin test for unsigned 64‑bit integers.      */
/* ------------------------------------------------------------------ */

static inline uint64_t modmul_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

static inline uint64_t modpow_uint64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t res = 1;
    while (d)
    {
        if (d & 1ULL) res = modmul_uint64(res, a, mod);
        a = modmul_uint64(a, a, mod);
        d >>= 1ULL;
    }
    return res;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;

    /* Small prime division. */
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* Decompose n‑1 as d·2ˢ. */
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0)
    {
        d >>= 1ULL;
        ++s;
    }

    /* Deterministic witnesses for 64‑bit integers. */
    static const uint64_t witnesses[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i)
    {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;

        uint64_t x = modpow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = modmul_uint64(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                            */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Silence unused‑parameter warning for builds where offset matters. */
    (void)n;

    /* Iterate over all natural numbers, selecting primes. */
    for (uint64_t p = 2; ; ++p)
    {
        if (!is_prime_uint64(p))
            continue;

        idx_t prod = (idx_t)p * n;         /* product fits in idx_t for n ≤ 10000 */

        seq_t sigma_val = sigma_of(prod);
        if ((sigma_val % (seq_t)n) == (seq_t)0)
            return (seq_t)p;
    }
}
