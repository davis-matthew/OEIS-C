/*
 * A286186  Number of connected induced (non-null) subgraphs of the friendship
 *          graph with 2n+1 nodes.
 * https://oeis.org/A286186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n + 3*n (offset 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 4^n */
    seq_t p = big_pow((seq_t)4, (seq_t)n);
    /* 3*n */
    seq_t t = (seq_t)3 * (seq_t)n;
    return p + t;
}
