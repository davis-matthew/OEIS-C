/*
 * A267816  Decimal representation of the n-th iteration of the "Rule 221"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) * (2^n - 1) - 1 (a(0) = 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;
    seq_t pown   = pow2(n);          /* 2^n   */
    seq_t pown1  = pow2(n + (idx_t)1);/* 2^(n+1) */
    return pown1 * (pown - (seq_t)1) - (seq_t)1;
}
