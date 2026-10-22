/*
 * A394384  Power tower of descending prime factors of n with multiplicity.
 * https://oeis.org/A394384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p₁^(p₂^(...^(p_k))) where p₁ is the largest prime factor
 *            of n and p_k is the smallest, each prime counted with
 *            multiplicity
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"

/* Return the smallest prime factor of m (m ≥ 2). */
static uint64_t _smallest_factor(uint64_t m)
{
    if ((m & 1) == 0) return 2;
    for (uint64_t d = 3; d <= m / d; d += 2) {
        if (m % d == 0) return d;
    }
    return m;   /* m is already prime */
}

seq_t a(idx_t n)
{
    /* a(1) = 1 as per OEIS offset. */
    if (n <= 1) return (seq_t)1;

    /* Factor n into primes with multiplicity,
       storing the factors in ascending order. */
    uint64_t factors[64];       /* enough for any n < 2⁶⁴ */
    size_t cnt = 0;
    uint64_t m = (uint64_t)n;

    while (m > 1) {
        uint64_t f = _smallest_factor(m);
        factors[cnt++] = f;
        m /= f;
    }

    /* Build the power tower: start with the smallest factor,
       then apply successive larger bases. */
    seq_t result = (seq_t)factors[0];
    for (size_t i = 1; i < cnt; ++i) {
        result = big_pow((seq_t)factors[i], result);
    }

    return result;
}
