/*
 * A284203  Number of twin prime (A001097) divisors of n.
 * https://oeis.org/A284203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n that are twin primes (a prime p
 *            such that either p-2 or p+2 is also prime)
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Integer square root (floor of sqrt) for 64‑bit numbers. */
static uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t low = 1, high = n >> 1, mid, sq;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)  low  = mid + 1;
        else         high = mid - 1;
    }
    return high;
}

/* 64‑bit modular multiplication using unsigned __int128 to avoid overflow. */
static uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* 64‑bit modular exponentiation. */
static uint64_t pow_mod_uint64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t res = 1;
    while (d) {
        if (d & 1) res = mul_mod_uint64(res, a, mod);
        a = mul_mod_uint64(a, a, mod);
        d >>= 1;
    }
    return res;
}

/* Deterministic Miller–Rabin test for 64‑bit integers. */
static bool is_prime_ull(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 = d * 2^s with d odd */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod_uint64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (cont) continue;
        return false;
    }
    return true;
}

/* Return true if x is a twin prime (a prime that has a prime
   neighbor at distance 2). */
static bool is_twin_prime(uint64_t x)
{
    if (!is_prime_ull(x)) return false;
    /* check x-2 */
    if (x > 2 && is_prime_ull(x - 2)) return true;
    /* check x+2, guard against overflow */
    if (x <= UINT64_MAX - 2 && is_prime_ull(x + 2)) return true;
    return false;
}

/* -------------------------------------------------------------------- */
/* Sequence function ---------------------------------------------------- */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset is 1 */
    uint64_t un = (uint64_t)n;
    seq_t count = 0;

    uint64_t limit = isqrt_u64(un);
    for (uint64_t d = 1; d <= limit; ++d) {
        if (un % d == 0) {
            uint64_t d1 = d;
            uint64_t d2 = un / d;

            if (d1 > 1 && is_twin_prime(d1)) ++count;
            if (d2 != d1 && d2 > 1 && is_twin_prime(d2)) ++count;
        }
    }
    return count;
}
