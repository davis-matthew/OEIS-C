/*
 * A255459  a(n) = A255458(2^n-1).
 * https://oeis.org/A255459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 - 2^(3+n) + 2*3^(1+n)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Defines a(idx_t n) with external linkage. */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t p2 = pow2(n + 3);        /* 2^(n+3) */
    seq_t p3 = pow3(n + 1);        /* 3^(n+1) */
    seq_t result = p3 * (seq_t)2;  /* 2*3^(n+1) */
    result -= p2;                  /* - 2^(n+3) */
    result += (seq_t)3;            /* + 3 */
    return result;
}
