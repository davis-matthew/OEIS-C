/*
 * A281773  Number of distinct topologies on an n-set that have exactly 4 open
 *          sets.
 * https://oeis.org/A281773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 - 5*2^(n-1) + 3^n (for n > 0)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   mul_small, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0 */
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* a(0) = 0 */

    /* Compute 3^n and 2^(n−1) using the provided helpers */
    seq_t p3 = pow3(n);                 /* 3^n */
    seq_t p2 = pow2(n - 1);             /* 2^(n−1) */

    /* Multiply 2^(n−1) by 5 */
    seq_t mul5 = mul_small(p2, 5);      /* 5 * 2^(n-1) */

    /* Compute 2 + 3^n - 5*2^(n-1) */
    seq_t result = p3;
    result += (seq_t)2;
    result -= mul5;

    return result;
}
