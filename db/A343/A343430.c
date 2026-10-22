/*
 * A343430  Part of n composed of prime factors of the form 3k-1.
 * https://oeis.org/A343430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product of prime factors of n that are congruent to 2 mod
 *            3 (each factor counted with its multiplicity)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t x = (uint64_t)n;

    /* Handle factor 2 separately */
    while ((x & 1ULL) == 0) {
        result *= (seq_t)2;
        x >>= 1;
    }

    /* Process odd factors */
    for (uint64_t p = 3; p * p <= x; p += 2) {
        while (x % p == 0) {
            if (p % 3ULL == 2ULL) {
                result *= (seq_t)p;
            }
            x /= p;
        }
    }

    /* Remaining prime factor, if any */
    if (x > 1 && x % 3ULL == 2ULL) {
        result *= (seq_t)x;
    }

    return result;
}
