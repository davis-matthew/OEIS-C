/*
 * A354108  a(n) = 1 if n is neither an odd prime power nor twice an odd prime
 *          power, otherwise 0.
 * https://oeis.org/A354108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is neither an odd prime power nor twice an odd *
 *            prime power; otherwise 0
 * Range:     u8 n <= 9210, u16 n <= 9210, u32 n <= 9210, u64 n <= 9210,
 *            u128 n <= 9210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Helper: Test whether n > 1 is an odd prime power (n = p^k with p odd prime). */
static inline bool is_odd_prime_power(idx_t n)
{
    if (n <= 1)
        return false;
    if ((n & 1ULL) == 0)        /* even numbers cannot be odd prime powers */
        return false;

    /* Find the smallest prime divisor of n. */
    idx_t p = 0;                /* will hold the smallest prime factor */
    if (n % 3ULL == 0) {
        p = 3;
    } else {
        idx_t d = 5;
        /* test divisors of the form 6k ± 1 */
        while (d <= n / d) {   /* avoid overflow */
            if (n % d == 0) {
                p = d;
                break;
            }
            d += 2;
            if (d > n / d)
                break;
            if (n % d == 0) {
                p = d;
                break;
            }
            d += 4;            /* skip multiples of 3 */
        }
    }
    if (p == 0)                 /* no divisor found → n is prime */
        p = n;

    /* Verify that p^k == n for some k >= 1. */
    idx_t m = n;
    while (m % p == 0)
        m /= p;
    return (m == 1);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)                  /* out of bounds (offset 1) – value not used */
        return (seq_t)0;

    if (is_odd_prime_power(n))
        return (seq_t)0;

    if ((n & 1ULL) == 0) {       /* n is even, test 2 × odd prime power */
        idx_t half = n >> 1;
        if (is_odd_prime_power(half))
            return (seq_t)0;
    }

    return (seq_t)1;
}
