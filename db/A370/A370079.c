/*
 * A370079  The product of the odd exponents of the prime factorization of n.
 * https://oeis.org/A370079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product of odd exponents in the prime factorisation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 by the OEIS offset */
    uint64_t nn = (uint64_t)n;

    /* Storage for the prime factorisation of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;                     /* number of prime factors */

    factor_u64(nn, primes, exps, &cnt);

    /* Multiply together all exponents that are odd */
    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] & 1ULL) {
            prod = prod * (seq_t)exps[i];
        }
    }
    return prod;
}
