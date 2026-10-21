/*
 * A278130  a(n) = 492*2^n - 222.
 * https://oeis.org/A278130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 492 * 2^n - 222
 * Range:     u8 none, u16 n <= 7, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Offset 0) */

/* This implementation uses the provided sequence utilities for arbitrary-precision
 * arithmetic.  The function computes the term in O(log n) big-integer
 * multiplications.  For builds where seq_t is an unsigned integer type,
 * the operations are defined by the underlying type. */

#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp)      */
#include "../utils/seq_mul.h"   /* seq_mul(seq_t a, seq_t b)          */

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t pow2 = seq_pow(2, n);

    /* Multiply by 492 and subtract 222 */
    seq_t res  = seq_mul(pow2, 492);
    res -= 222;

    return res;
}
