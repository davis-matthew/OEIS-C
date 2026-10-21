/*
 * A293449  Characteristic function for A056166, numbers that have no nonprime
 *          exponents present in their prime factorization n = p_1^e_1 * ... *
 *          p_k^e_k.
 * https://oeis.org/A293449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if every exponent in the prime factorization of n is a
 *            prime, otherwise a(n) = 0. a(1) = 1. This implements the
 *            characteristic function for A056166
 * Range:     u8 n <= 9603, u16 n <= 9603, u32 n <= 9603, u64 n <= 9603,
 *            u128 n <= 9603, big any n
 * Helpers:   _smallest_prime_factor, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 1 by definition. */
    if (n <= 1)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;

    while (m > 1) {
        uint64_t p = _smallest_prime_factor(m);
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            exp++;
        }
        if (!is_prime_small((seq_t)exp))
            return (seq_t)0;
    }
    return (seq_t)1;
}
