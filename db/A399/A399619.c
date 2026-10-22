/*
 * A399619  Number of ordered n-edge multihypergraphs on n labeled vertices
 *          with no isolated vertex and with at least one vertex contained in
 *          every edge.
 * https://oeis.org/A399619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n - 1)^n - (2^n - 2)^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* For n < 1 the offset of the sequence does not matter. */
    if (n < 1)
        return 0;

    seq_t two_n = pow2(n);                 /* 2^n */
    seq_t one   = (seq_t)1;
    seq_t two   = (seq_t)2;

    seq_t base1 = two_n - one;             /* 2^n - 1 */
    seq_t base2 = two_n - two;             /* 2^n - 2 */

    seq_t p1 = big_pow(base1, (seq_t)n);   /* (2^n - 1)^n */
    seq_t p2 = big_pow(base2, (seq_t)n);   /* (2^n - 2)^n */

    return p1 - p2;                        /* difference */
}
