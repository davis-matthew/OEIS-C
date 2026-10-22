/*
 * A332184  a(n) = 8*(10^(2*n+1) - 1)/9 - 4*10^n.
 * https://oeis.org/A332184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 8*(10^(2*n+1) - 1)/9 - 4*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* 10^(2*n+1) */
    idx_t exp = 2 * n + 1;
    seq_t pow_exp = pow10(exp);

    /* 10^n */
    seq_t pow_n = pow10(n);

    /* part1 = 8*(pow_exp - 1)/9  (division is exact) */
    seq_t t      = pow_exp - 1;    /* 10^(2*n+1) - 1 */
    seq_t t_div9 = t / 9;         /* exact division by 9 */
    seq_t part1  = t_div9 * 8;     /* multiply after division */

    /* part2 = 4*10^n */
    seq_t part2 = pow_n * 4;

    return part1 - part2;
}
