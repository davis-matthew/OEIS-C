/*
 * A400140  The number of divisors d of n such that gcid(d, n/d) is a square,
 *          where gcid is the greatest common infinitary divisor.
 * https://oeis.org/A400140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_per_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A400140: Number of divisors d of n such that the greatest
 * common infinitary divisor of d and n/d is a square.
 *
 * The sequence is multiplicative.  For n = ∏p^e the value is
 * a(n) = Π a(p^e), where a(p^e) is the number of integers
 * 0 ≤ b ≤ e whose intersection exponent b & (e − b) is even.
 * This is computed directly from the prime factorisation of n.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/count_per_prime.h"

/* main function for the sequence                                                        */
seq_t a(idx_t n)
{
    /* handle n = 1 explicitly (an empty product) */
    if (n <= 1) return 1;

    uint64_t m = (uint64_t)n;   /* factor n as uint64_t */
    seq_t result = 1;

    /* trial division up to sqrt(m) */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            result *= count_per_prime(exp);
        }
    }

    /* any remaining factor m > 1 is a prime with exponent 1 */
    if (m > 1) {
        result *= count_per_prime(1);
    }

    return result;
}
