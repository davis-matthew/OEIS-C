/*
 * A386258  Exponent of the highest power of 2 dividing the product of
 *          exponents of the prime factorization of n.
 * https://oeis.org/A386258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of the exponents of 2 in the exponents of the prime
 *            factorization of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* that is, a(n) = v2(e1) + v2(e2) + ... for n = p1^e1 * p2^e2 * ... */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;                /* a(1) = 0 */

    uint64_t m = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;

    /* factor_u64 fills primes[], exps[] and stores the number of distinct prime factors in cnt */
    factor_u64(m, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        /* add v2(e) to the result */
        while ((e & 1ULL) == 0 && e != 0) {
            ++sum;
            e >>= 1;
        }
    }
    return sum;
}
