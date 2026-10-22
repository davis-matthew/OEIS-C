/*
 * A361631  a(n) is the denominator of the median of the distinct prime
 *          factors of n.
 * https://oeis.org/A361631
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) is the denominator of the median of the distinct prime
 *            factors of n. For even number of distinct primes, the median is
 *            (p_{k-1}+p_k)/2, whose denominator is 1 if the sum is even, 2
 *            otherwise. For odd number of distinct primes, the median is an
 *            integer prime, so the denominator is 1. For a pr...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2.  For safety we handle
       n <= 1 by returning 1, although it is outside the offset. */
    if (n <= 1) return (seq_t)1;

    /* Factor n into distinct primes. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* If there is at most one distinct prime, the denominator is 1. */
    if (cnt <= 1) return (seq_t)1;

    /* For an odd number of distinct primes the median is an integer. */
    if (cnt & 1) return (seq_t)1;

    /* Else we have an even number of primes.  Determine parity of
       the sum of the two middle primes. */
    uint64_t mid1 = primes[cnt / 2 - 1];
    uint64_t mid2 = primes[cnt / 2];

    /* If the two middle primes have different parity, their sum is
       odd and the denominator is 2; otherwise the sum is even and the
       denominator is 1. */
    return (((mid1 & 1ULL) ^ (mid2 & 1ULL)) ? (seq_t)2 : (seq_t)1);
}
