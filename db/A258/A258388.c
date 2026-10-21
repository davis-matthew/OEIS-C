/*
 * A258388  a(n) = n^(n+1) + (n-1)^n.
 * https://oeis.org/A258388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^(n+1) + (n-1)^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t sn = (seq_t)n;

    /* n^(n+1) */
    seq_t part1 = big_pow(sn, sn + (seq_t)1);

    /* (n-1)^n, safe because n > 0 */
    seq_t snm1 = sn - (seq_t)1;
    seq_t part2 = big_pow(snm1, sn);

    return part1 + part2;
}
