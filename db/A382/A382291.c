/*
 * A382291  a(n) = A037445(n)/A034444(n).
 * https://oeis.org/A382291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 2^{∑_{p^e∥n} (popcount(e)−1)}. That is, a is
 *            multiplicative with a(p^e) = 2^{popcount(e)−1}
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
    /* n ≥ 1; for n = 1 the result is 1. */
    if (n <= 1)
        return (seq_t)1;

    /* factor_u64 writes into these arrays (at most 15 distinct primes in 64‑bit). */
    static uint64_t pr[20];
    static uint64_t ex[20];
    size_t cnt = 0;

    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = ex[i];
        unsigned int pc   = __builtin_popcountll(e); /* popcount(e) */
        unsigned int shift = pc - 1;                 /* exponent of 2 */

        for (unsigned int s = 0; s < shift; ++s)
            result <<= 1;
    }

    return result;
}
