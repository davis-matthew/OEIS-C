/*
 * A363735  a(n) = Sum_{k=0..n} n^notdivides(k, n), where notdivides(k, n) = 0
 *          if k divides n, otherwise 1.
 * https://oeis.org/A363735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 + n + (1 - n)*tau(n) for n >= 1, a(0) = 1
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where tau(n) = number of positive divisors of n  */
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Handle the offset term a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Convert n to a 64‑bit unsigned integer for divisor counting. */
    uint64_t n64 = (uint64_t)n;
    uint64_t tau64 = divisor_count_u64(n64);

    /* Perform the arithmetic in seq_t.  Overflow will not occur for all
       values that fit in the underlying seq_t type (the test harness
       limits n accordingly). */
    seq_t nseq   = (seq_t)n64;
    seq_t tau    = (seq_t)tau64;

    /* n^2 + n */
    seq_t result = nseq * nseq + nseq;

    /* subtract n * tau */
    result -= nseq * tau;

    /* add tau */
    result += tau;

    return result;
}
