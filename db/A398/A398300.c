/*
 * A398300  a(n) is the minimum possible sum of the numbers on all non-mine
 *          cells when n mines are placed on an infinite Minesweeper board
 *          (each non-mine cell's number is the count of mines among its up to
 *          8 neighbors).
 * https://oeis.org/A398300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*ceiling(sqrt(28*n - 12))
 * Range:     u8 n <= 2, u16 n <= 483, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* 28*n - 12 in seq_t */
    seq_t t = (seq_t)28 * (seq_t)n - (seq_t)12;

    /* floor sqrt */
    seq_t s = floor_sqrt(t);

    /* if not perfect square, take ceil */
    if (s * s < t) s += 1;

    return (seq_t)2 * s;
}
