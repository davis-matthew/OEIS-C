/*
 * A382677  a(n) = 9 - 28 * 2^n + 20 * 3^n.
 * https://oeis.org/A382677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9 - 28 * 2^n + 20 * 3^n
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 78,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

/* Computes a(n) = 9 - 28·2^n + 20·3^n  (offset 0) */
seq_t a(idx_t n)
{
    seq_t p2 = pow2(n);             /* 2^n */
    seq_t p3 = pow3(n);             /* 3^n */
    seq_t temp = ((seq_t)20 * p3)   /* 20·3^n  */
                 + (seq_t)9;        /* +9      */
    seq_t result = temp - ((seq_t)28 * p2); /* -28·2^n */
    return result;
}
