/*
 * A383277  The number of divisors d of n for which A034444(d)*d is equal to
 *          n.
 * https://oeis.org/A383277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A383277:
   a(n) = 1 if there exists a divisor d of n such that
          2^ω(d) * d = n, otherwise 0.
   Owing to the constraints, at most one such divisor can exist.
   The algorithm tests the parity exponent of n and the count
   of distinct odd prime factors of its odd part. */
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Extract the exponent of 2 in n and the odd part m. */
    idx_t e = 0;
    uint64_t nn = (uint64_t)n;
    while ((nn & 1ULL) == 0) {
        ++e;
        nn >>= 1;
    }
    uint64_t m = nn;          /* odd part of n */

    /* Count distinct odd primes dividing m. */
    size_t cnt = 0;
    uint64_t pr[64], ex[64];
    size_t t = 0;             /* number of distinct odd primes */

    if (m != 1ULL) {
        factor_u64(m, pr, ex, &cnt);
        for (size_t i = 0; i < cnt; ++i)
            if (pr[i] != 2ULL)
                ++t;
    }

    /* Check the two conditions that guarantee the existence
       of the desired divisor. */
    if (e == t)                /* k = e case */
        return (seq_t)1;
    if (t + 1ULL < e)          /* k = t+1 case, with k < e */
        return (seq_t)1;

    return (seq_t)0;
}
