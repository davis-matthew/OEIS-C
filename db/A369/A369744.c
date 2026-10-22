/*
 * A369744  a(n) = Sum_{p|n, p prime} p * omega(n/p).
 * https://oeis.org/A369744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{p|n, p prime} p * ω(n/p)
 * Range:     u8 n <= 513, u16 n <= 8901, u32 n <= 8901, u64 n <= 8901,
 *            u128 n <= 8901, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where ω(m) = number of distinct prime factors of m (ω(1)=0). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition. */
    if (n <= 1)
        return 0;

    /* Factor n into distinct primes and their exponents. */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        /* ω(n/p) is cnt if the exponent of p ≥ 2, else cnt‑1. */
        seq_t omega = (exps[i] >= 2) ? (seq_t)cnt : (seq_t)(cnt - 1);
        sum += (seq_t)primes[i] * omega;
    }
    return sum;
}
