/*
 * A375847  The maximum exponent in the prime factorization of the largest
 *          unitary cubefree divisor of n.
 * https://oeis.org/A375847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n is cubefull; 2 if any prime has exponent 2 in n;
 *            otherwise 1 if any prime has exponent 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t
a(idx_t n)
{
    /* special case n = 1 -> no prime factors -> 0 */
    if (n == 1)
        return (seq_t)0;

    /* arrays large enough for all distinct prime factors of a 64‑bit integer */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor the integer n (n is converted to uint64_t) */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    int has1 = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] == 2)          /* any exponent 2 gives 2 */
            return (seq_t)2;
        if (exps[i] == 1)
            has1 = 1;              /* record a 1 if present */
    }

    return has1 ? (seq_t)1 : (seq_t)0;   /* 0 if all exponents >=3 */
}
