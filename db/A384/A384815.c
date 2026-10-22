/*
 * A384815  Sum of the cubes of the exponents in the prime factorization of n.
 * https://oeis.org/A384815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ (k_j³) over prime powers p_j^k_j that divide n
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sum of cubes of the exponents in the prime factorization
   of n.  The function works for any integer type assigned to seq_t
   (uint8_t, uint16_t, uint32_t, uint64_t, unsigned __int128, or an
   arbitrary‑precision integer class). */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* For n < 2 there are no prime factors, so return 0. */
    if (n < 2)
        return (seq_t)0;

    seq_t result = 0;
    uint64_t x = (uint64_t)n;   /* work with a normal integer type */
    uint64_t exp = 0;

    /* Handle factor 2 separately. */
    while ((x & 1ULL) == 0) {
        exp++;
        x >>= 1;
    }
    if (exp)
        result += (seq_t)(exp * exp * exp);

    /* Iterate over odd divisors up to sqrt(x). */
    for (uint64_t p = 3; p * p <= x; p += 2) {
        exp = 0;
        while (x % p == 0) {
            exp++;
            x /= p;
        }
        if (exp)
            result += (seq_t)(exp * exp * exp);
    }

    /* If x > 1 now, it is a prime with exponent 1. */
    if (x > 1)
        result += (seq_t)1;   /* 1³ = 1 */

    return result;
}
