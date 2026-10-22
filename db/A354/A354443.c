/*
 * A354443  a(n) = Fibonacci(n^n) mod n.
 * https://oeis.org/A354443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Fibonacci(n^n) mod n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_uint64, prime_pisano
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A354443.  */
#include <stdint.h>
#include "../utils/gcd_uint64.h"
#include "../utils/prime_pisano.h"

/* small utility: integer gcd */

/* compute x^e (unsigned exponent) */
static uint64_t pow_uint64(uint64_t x, uint64_t e)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1ULL)
            r *= x;
        x *= x;
        e >>= 1;
    }
    return r;
}

/* modular exponentiation modulo mod */
static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1ULL)
        return 0;
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* compute Pisano period of a prime power p^k */
static uint64_t prime_power_pisano(uint64_t p, uint64_t k)
{
    if (p == 2ULL) {
        if (k == 1ULL)       return 3ULL;
        if (k == 2ULL)       return 6ULL;
        return 3ULL << (k - 1ULL);        /* 3 * 2^(k-1) */
    }
    if (p == 5ULL) {
        return 20ULL * pow_uint64(5ULL, k - 1ULL);
    }
    uint64_t pi_p = prime_pisano(p);
    return pi_p * pow_uint64(p, k - 1ULL);
}

/* compute Pisano period of an arbitrary n (n>=1) using prime factorisation */
static uint64_t pisano_period(uint64_t n)
{
    if (n == 1ULL)
        return 1ULL;

    uint64_t k = n;
    uint64_t lcm_val = 1ULL;
    for (uint64_t p = 2ULL; p * p <= k; ++p) {
        if (k % p == 0) {
            uint64_t exp = 0;
            while (k % p == 0) {
                k /= p;
                ++exp;
            }
            uint64_t period = prime_power_pisano(p, exp);
            lcm_val = lcm_val / gcd_uint64(lcm_val, period) * period;
        }
    }
    if (k > 1ULL) { /* remaining prime factor */
        uint64_t period = prime_power_pisano(k, 1ULL);
        lcm_val = lcm_val / gcd_uint64(lcm_val, period) * period;
    }
    return lcm_val;
}

/* fast doubling Fibonacci modulo mod: returns F(n) and F(n+1) */
static void fib_pair(uint64_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % mod;
        return;
    }
    uint64_t a, b;
    fib_pair(n >> 1, mod, &a, &b);          /* a = F(k), b = F(k+1) */

    uint64_t two_b = b << 1;                 /* 2*b */
    if (two_b >= mod) two_b %= mod;
    uint64_t temp = (two_b + mod - a) % mod; /* 2*b - a  (mod) */

    uint64_t c = (unsigned __int128)a * temp % mod;                                   /* F(2k) */
    uint64_t d = ((unsigned __int128)a * a + (unsigned __int128)b * b) % mod;         /* F(2k+1) */

    if ((n & 1ULL) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = (c + d) % mod;
    }
}

/* main sequence function as requested */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* out of offset returns 0 */

    uint64_t mod   = (uint64_t)n;                  /* n is <1e4, fits 64-bit */
    uint64_t pi_n  = pisano_period(mod);           /* <= 6*mod <= 60000 */
    uint64_t e_mod = powmod_uint64(mod, mod, pi_n);/* compute n^n mod pi(n) */

    uint64_t fn, fn1;
    fib_pair(e_mod, mod, &fn, &fn1);              /* Fibonacci(e_mod) mod n */
    return (seq_t)(fn % mod);
}
