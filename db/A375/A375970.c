/*
 * A375970  a(n) is the largest number k such that k^2 divides the square
 *          pyramidal number A000330(n).
 * https://oeis.org/A375970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k such that k² divides n*(n+1)*(2n+1)/6 (i.e.
 *            a(n) = sqrt( square part of n(n+1)(2n+1)/6 )
 * Range:     u8 n <= 721, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* offset is 1, so n>=1 in OEIS */
    }

    /* compute the square pyramidal number P(n) using 64‑bit arithmetic */
    uint64_t N = (uint64_t)n;
    uint64_t P = N * (N + 1) * (2 * N + 1) / 6;

    /* factor P(n); the number of distinct prime factors is small */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(P, primes, exps, &cnt);

    /* build a(n) as product of primes^{floor(exp/2)} */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        unsigned half = (unsigned)(exps[i] / 2);
        for (unsigned j = 0; j < half; ++j) {
            result *= (seq_t)primes[i];
        }
    }
    return result;
}
