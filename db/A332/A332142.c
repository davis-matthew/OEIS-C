/*
 * A332142  a(n) = 4*(10^(2*n+1)-1)/9 - 2*10^n.
 * https://oeis.org/A332142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*(10^(2*n+1)-1)/9 - 2*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    idx_t exp_big = 2 * n + 1;
    seq_t pow_big = pow10(exp_big);

    seq_t diff   = pow_big - (seq_t)1;
    seq_t div9   = diff / (seq_t)9;
    seq_t part1  = div9 * (seq_t)4;

    seq_t part2  = pow10(n) * (seq_t)2;

    return part1 - part2;
}
