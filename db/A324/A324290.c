/*
 * A324290  a(n) = 1 if for every prime divisor p of n, p-1 divides n-1, 0
 *          otherwise; characteristic function of A087441.
 * https://oeis.org/A324290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if for every prime divisor p of n, p-1 divides n-1, 0
 *            otherwise
 * Range:     u8 n <= 9462, u16 n <= 9462, u32 n <= 9462, u64 n <= 9462,
 *            u128 n <= 9462, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Euclidean gcd on uint64_t. */
static uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b)
    {
        uint64_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    /* By definition a(1) = 1 (vacuous truth). */
    if (n <= 1)
        return 1;

    uint64_t m = (uint64_t)n;          /* working copy for factorisation */
    uint64_t n_minus_1 = m - 1;        /* n-1 fits in 64 bits, n up to 2^64-1 */
    uint64_t lcm = 1;                  /* lcm of (p-1) over prime divisors p */

    /* Trial division to find prime divisors. */
    for (idx_t i = 2; i <= m / i; ++i)
    {
        if (m % i == 0)
        {
            /* i is a prime divisor of the original n. */
            uint64_t p = i;
            uint64_t d = p - 1;        /* p-1 */

            /* Update lcm safely using 128‑bit arithmetic. */
            uint64_t g = gcd64(lcm, d);
            __uint128_t t = (__uint128_t)(lcm / g) * d;
            if (t > n_minus_1)
                return 0;              /* lcm already exceeds n-1 */

            lcm = (uint64_t)t;

            /* Divide out all powers of p. */
            while (m % i == 0)
                m /= i;
        }
    }

    /* If after the loop m > 1 then m itself is a prime divisor. */
    if (m > 1)
    {
        uint64_t p = m;
        uint64_t d = p - 1;
        uint64_t g = gcd64(lcm, d);
        __uint128_t t = (__uint128_t)(lcm / g) * d;
        if (t > n_minus_1)
            return 0;

        lcm = (uint64_t)t;
    }

    /* Final check: n-1 must be a multiple of the lcm of all (p-1). */
    return ((n_minus_1 % lcm) == 0) ? 1 : 0;
}
