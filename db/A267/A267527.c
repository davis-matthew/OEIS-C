/*
 * A267527  Decimal representation of the n-th iteration of the "Rule 141"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (4^⌊n/2⌋ - 1) / 3 + 2^(n-1) - 1 for n > 1 with a(0)
 *            = 1, a(1) = 2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Small index special cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    /* k = ⌊n/2⌋ */
    idx_t k = n >> 1;

    /* 4^k = 2^(2k) */
    seq_t pow4k = pow2(2 * k);

    /* (4^k – 1) / 3 */
    seq_t div3 = (pow4k - (seq_t)1) / (seq_t)3;

    /* 2^n * ((4^k - 1) / 3) */
    seq_t part1 = pow2(n) * div3;

    /* 2^(n-1) - 1 */
    seq_t part2 = pow2(n - 1) - (seq_t)1;

    return part1 + part2;
}
