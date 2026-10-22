/*
 * A303915  a(n) = lambda(n)*E(n), where lambda(n) = A008836(n) and E(n) =
 *          A005361(n).
 * https://oeis.org/A303915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = A008836(n) * A005361(n) (lambda(n) * product of prime
 *            exponents)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;

    /* maximum number of distinct prime factors for 64‑bit n is < 65 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod = (seq_t)1;
    unsigned parity = 0;                /* parity of sum of exponents (λ(n) sign) */

    for (size_t i = 0; i < cnt; ++i) {
        prod = prod * (seq_t)exps[i];
        parity ^= (unsigned)(exps[i] & 1U);
    }

    if (parity) prod = -prod;
    return prod;
}
