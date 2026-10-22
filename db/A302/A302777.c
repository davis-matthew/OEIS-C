/*
 * A302777  a(n) = 1 if n is of the form p^(2^k) where p is prime and k >= 0,
 *          otherwise 0.
 * https://oeis.org/A302777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9621, u16 n <= 9621, u32 n <= 9621, u64 n <= 9621,
 *            u128 n <= 9621, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recognizes whether n is of the form p^(2^k) (k≥0, p prime).
   Returns 1 if so, otherwise 0. Implements OEIS A302777. */

#include <stdbool.h>
#include <stdint.h>

static bool is_power_of_two(uint64_t x)
{
    return x && !(x & (x - 1));
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;          /* a(1) = 0 */

    uint64_t m   = (uint64_t)n;   /* work on a 64‑bit copy of n */
    uint64_t tmp = m;
    uint64_t prime = 0;

    /* Find the smallest prime factor of n. */
    if ((tmp & 1ULL) == 0) {
        prime = 2;
    } else {
        for (uint64_t d = 3; d <= tmp / d; d += 2) {
            if (tmp % d == 0) {
                prime = d;
                break;
            }
        }
    }

    /* If no divisor was found, n itself is prime; exponent = 1. */
    if (prime == 0)
        return (seq_t)1;

    /* Count the exponent of the prime factor. */
    unsigned int exp = 0;
    while (m % prime == 0) {
        m   /= prime;
        exp += 1;
    }

    /* If anything remains, n had more than one prime factor. */
    if (m != 1)
        return (seq_t)0;

    /* Check if the exponent is a power of two. */
    return (seq_t)(is_power_of_two(exp) ? 1 : 0);
}
