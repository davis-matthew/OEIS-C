/*
 * A336898  a(n) = numerator(n / (4^n - 2^n)) for n > 0 and a(0) = 1.
 * https://oeis.org/A336898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator(n / (4^n - 2^n)) = n / gcd(n, 4^n - 2^n)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast binary exponentiation modulo modulus, with 128-bit intermediate product */
static uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = (uint64_t)((__uint128_t)result * b % mod);
        b = (uint64_t)((__uint128_t)b * b % mod);
        exp >>= 1ULL;
    }
    return result;
}

/* Euclidean algorithm for 64‑bit unsigned integers */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b)
    {
        uint64_t tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* compute (4^n - 2^n) mod n */
    uint64_t mod = (uint64_t)n;          /* n fits in uint64_t */
    uint64_t pow2 = pow_mod_uint64(2ULL, mod, mod);
    uint64_t pow4 = pow_mod_uint64(4ULL, mod, mod);
    uint64_t r = (pow4 + mod - pow2) % mod;   /* r = (4^n - 2^n) mod n */

    uint64_t g = gcd_uint64((uint64_t)n, r);   /* gcd(n, 4^n-2^n) */
    uint64_t result = (uint64_t)n / g;         /* a(n) = n/g */

    return (seq_t)result;
}
