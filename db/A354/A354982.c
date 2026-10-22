/*
 * A354982  a(n) = 1 if n is a prime power congruent to 1 (mod 3), otherwise
 *          0.
 * https://oeis.org/A354982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a prime power congruent to 1 (mod 3), otherwise
 *            0
 * Range:     u8 n <= 9884, u16 n <= 9884, u32 n <= 9884, u64 n <= 9884,
 *            u128 n <= 9884, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1u) {          /* a(1) = 0 and a(0) is irrelevant */
        return (seq_t)0;
    }

    uint64_t temp = (uint64_t)n;
    uint64_t p;

    /* Try all possible prime factors up to sqrt(temp) */
    for (p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            /* Found a prime factor p; divide out all its copies */
            while (temp % p == 0) {
                temp /= p;
            }
            /* If more than one distinct prime factor remained, stop */
            if (temp != 1) {
                return (seq_t)0;
            }
            /* temp == 1 -> n = p^k ; check the congruence */
            return ((n % 3ULL) == 1ULL) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* No divisor found → n is prime */
    return ((n % 3ULL) == 1ULL) ? (seq_t)1 : (seq_t)0;
}
