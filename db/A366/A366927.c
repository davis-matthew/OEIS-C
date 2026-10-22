/*
 * A366927  Number of distinct primes p used in the mapping of k = k - k/p
 *          terminating at 1, where p is any of the prime factors of k.
 * https://oeis.org/A366927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct primes occurring while repeatedly
 *            replacing k by k - k/p for a prime factor p of k until k becomes
 *            1. a(1)=0
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  All helper routines use only 64‑bit arithmetic; the result is
    returned as seq_t.                                                */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd64.h"

/* ---------- deterministic 64‑bit modular arithmetic ---------- */
static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1 % mod;
    a %= mod;
    while (e) {
        if (e & 1) r = mul_mod(r, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* ---------- simple GCD ---------------------- */

/* ---------- deterministic Miller–Rabin for 64‑bit ------------ */
static bool is_prime_64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_prime[8] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL};
    static const size_t sp_cnt = sizeof(small_prime)/sizeof(small_prime[0]);
    for (size_t i=0; i<sp_cnt; ++i) {
        uint64_t p = small_prime[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1; ++s;
    }
    static const uint64_t bases[7] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    static const size_t bases_cnt = sizeof(bases)/sizeof(bases[0]);
    for (size_t i=0; i<bases_cnt; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool comp = true;
        for (int r=1; r<s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n-1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ---------- simple linear‑congruential RNG for Pollard-Rho ---- */
static uint64_t rng_state = 88172645463393265ULL;

static uint64_t rng64(void)
{
    rng_state ^= rng_state << 7;
    rng_state ^= rng_state >> 9;
    return rng_state;
}

/* ---------- Pollard's Rho ----------------------------------- */
static uint64_t pollard(uint64_t n)
{
    if ((n & 1) == 0) return 2;
    while (1) {
        uint64_t c = rng64() % (n - 1) + 1;
        uint64_t x = rng64() % (n - 2) + 2;
        uint64_t y = x;
        uint64_t d = 1;
        while (d == 1) {
            x = (mul_mod(x, x, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            y = (mul_mod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd64(diff, n);
        }
        if (d != n) return d;
    }
}

/* ---------- smallest prime factor (recursive) -------------- */
static uint64_t spf(uint64_t n)
{
    if ((n & 1) == 0) return 2;
    static const uint64_t small_primes[] =
        {3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,
         37ULL,41ULL,43ULL,47ULL,53ULL,59ULL,61ULL,67ULL,71ULL,73ULL,
         79ULL,83ULL,89ULL,97ULL};
    static const size_t small_cnt = sizeof(small_primes)/sizeof(small_primes[0]);

    for (size_t i=0; i<small_cnt; ++i) {
        uint64_t p = small_primes[i];
        if (p*p > n) break;
        if (n % p == 0) return p;
    }
    if (n < 1000000ULL) {
        for (uint64_t p=3; p*p <= n; p+=2) {
            if (n % p == 0) return p;
        }
        return n; /* prime */
    }
    if (is_prime_64(n)) return n;
    uint64_t d = pollard(n);
    return spf(d);
}

/* -------------------------- main function ------------------ */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    uint64_t k = (uint64_t)n;
    uint64_t seen[64];
    size_t seen_cnt = 0;

    while (k > 1) {
        uint64_t p = spf(k);
        /* add p if not already seen */
        bool already = false;
        for (size_t i=0; i<seen_cnt; ++i) {
            if (seen[i] == p) { already = true; break; }
        }
        if (!already) {
            seen[seen_cnt++] = p;
        }
        k -= k / p;
    }
    return (seq_t)seen_cnt;
}
