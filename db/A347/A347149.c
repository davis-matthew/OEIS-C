/*
 * A347149  Dirichlet g.f.: Product_{primes p} (1 + 3/p^s).
 * https://oeis.org/A347149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 3^(ω(n)) if n is squarefree, otherwise 0 (a(1)=1). Here
 *            ω(n) is the number of distinct prime factors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
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
    if (n == 1)
        return (seq_t)1;

    uint64_t tn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* factor_u64 fills primes[], exps[] and sets cnt */
    factor_u64(tn, primes, exps, &cnt);

    /* if any exponent > 1, n is not squarefree */
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] > 1)
            return (seq_t)0;
    }

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
        result *= (seq_t)3;

    return result;
}
