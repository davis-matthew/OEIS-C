/*
 * A330569  a(n) = 1 if n is odd, otherwise a(n) = 2^(v-1)+1 where v is the
 *          2-adic valuation of n (A007814(n)).
 * https://oeis.org/A330569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 if n is odd, otherwise a(n) = 2^(v2(n)-1) + 1 where
 *            v2(n) is the exponent of the largest power of 2 dividing n
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the 2-adic valuation v2(n) for n >= 1. */
static inline idx_t v2(idx_t n)
{
    idx_t v = 0;
    while ((n & (idx_t)1) == 0)
    {
        n >>= 1;
        ++v;
    }
    return v;
}

/* OEIS sequence A330569. */
seq_t a(idx_t n)
{
    /* Odd indices yield 1. */
    if ((n & (idx_t)1) != 0)
        return (seq_t)1;

    /* Even n: compute 2^(v-1) + 1 where v = v2(n). */
    idx_t v = v2(n);          /* v >= 1 for even n */
    seq_t val = (seq_t)1;
    /* Multiply by 2, v-1 times. */
    for (idx_t i = 1; i < v; ++i)
        val *= (seq_t)2;

    return val + (seq_t)1;
}
