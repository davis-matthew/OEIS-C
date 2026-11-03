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
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 9603 (2026-11-01)
 * Helpers:   is_prime_seq, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_seq.h"
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 1 by definition. */
    if (n <= 1)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;

    while (m > 1) {
        uint64_t p = smallest_prime_factor(m);
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            exp++;
        }
        if (!is_prime_seq((seq_t)exp))
            return (seq_t)0;
    }
    return (seq_t)1;
}
