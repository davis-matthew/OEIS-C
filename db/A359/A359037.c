/*
 * A359037  a(n) = Sum_{d|n} tau(d^6), where tau(n) = number of divisors of n,
 *          cf. A000005.
 * https://oeis.org/A359037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = tau(n) * tau(n^3)
 * Range:     u8 n <= 23, u16 n <= 4619, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safe guard for completeness */

    /* prime factorisation arrays – 64 slots suffice for any 64‑bit n */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t cnt = 0;

    /* get prime factorisation of n */
    factor_u64((uint64_t)n, primes, exponents, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e   = (seq_t)exponents[i];
        seq_t term = (e + 1) * (3 * e + 1);   /* (tau(p^e))*(tau(p^{3e})) */
        result *= term;
    }
    return result;
}
