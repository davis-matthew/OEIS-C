/*
 * A250750  Number of (n+1) X (3+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)+x(i-1,j) in the j direction.
 * https://oeis.org/A250750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (114*3^n -48*2^n +12)/2
 * Range:     u8 none, u16 n <= 5, u32 n <= 15, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n>=1 using the closed‑form expression. */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; for n<1 the value is undefined. */
    if (n == 0) return (seq_t)0;          /* guard against accidental 0 */

    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);   /* 3^n */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);   /* 2^n */

    seq_t result = (seq_t)114 * pow3;          /* 114*3^n */
    result -= (seq_t)48 * pow2;                /* -48*2^n */
    result += (seq_t)12;                       /* +12 */
    result /= (seq_t)2;                        /* divide by 2 (exact) */

    return result;
}
