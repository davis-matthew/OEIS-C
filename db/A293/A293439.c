/*
 * A293439  Number of odious exponents in the prime factorization of n.
 * https://oeis.org/A293439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of odious exponents in the prime factorization of
 *            n (an exponent e is odious if popcount(e) is odd)
 * Range:     u8 n <= 9302, u16 n <= 9302, u32 n <= 9302, u64 n <= 9302,
 *            u128 n <= 9302, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* 1 has no prime factors → a(1) = 0 */
    if (n <= 1)
        return 0;

    /* Factorization buffer: 64 slots are enough for all 64‑bit integers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t odious = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = exps[i];
        /* popcount(e): odd => odious exponent */
        if (__builtin_popcountll(e) & 1)
            ++odious;
    }
    return odious;
}
