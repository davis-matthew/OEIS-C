/*
 * A375339  If n has exactly one non-unitary prime factor then a(n) is the
 *          exponent of the highest power of this prime that divides n,
 *          otherwise a(n) = 0.
 * https://oeis.org/A375339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the exponent of the unique prime factor of n that is
 *            raised to a power >= 2; if no such prime exists a(n)=0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd_u64.h"

/* ---------- utilities ---------- */

static uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1 % mod;
    while (e)
    {
        if (e & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool is_prime(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mul_mod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* xorshift64* RNG, deterministic and fast in both C and C++ */
static uint64_t rnd_state = 88172645463393265ULL;
static uint64_t rand64(void)
{
    rnd_state ^= rnd_state << 13;
    rnd_state ^= rnd_state >> 7;
    rnd_state ^= rnd_state << 17;
    return rnd_state;
}

/* Pollard-Rho factorisation */
static uint64_t pollard_rho(uint64_t n)
{
    if ((n & 1ULL) == 0) return 2;
    while (1)
    {
        uint64_t c = rand64() % (n - 1) + 1;
        uint64_t x = rand64() % n;
        uint64_t y = x;
        uint64_t d = 1;
        while (d == 1)
        {
            x = (mul_mod(x, x, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
            if (d == n) break;
        }
        if (d > 1 && d < n) return d;
    }
}

/* recursive factorisation, collecting primes and exponents */
static void factor_rec(uint64_t n, size_t *cnt, uint64_t pr[64], uint32_t ex[64])
{
    if (n == 1) return;
    if (is_prime(n))
    {
        for (size_t i = 0; i < *cnt; ++i)
        {
            if (pr[i] == n)
            {
                ++ex[i];
                return;
            }
        }
        pr[*cnt] = n;
        ex[*cnt] = 1;
        ++(*cnt);
        return;
    }
    uint64_t d = pollard_rho(n);
    factor_rec(d, cnt, pr, ex);
    factor_rec(n / d, cnt, pr, ex);
}

/* ---------- sequence function ---------- */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    uint64_t nn = (uint64_t)n;

    size_t cnt = 0;
    uint64_t pr[64];
    uint32_t ex[64];
    factor_rec(nn, &cnt, pr, ex);

    uint32_t e_single = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        if (ex[i] >= 2)
        {
            if (e_single != 0)   /* more than one exponent >= 2 */
                return 0;
            e_single = ex[i];
        }
    }
    return (seq_t)e_single;
}
