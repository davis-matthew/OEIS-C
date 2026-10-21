/*
 * A267879  Binary representation of the middle column of the "Rule 233"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9091*10^(n-3) - 1)/9 for n > 2 Equivalent: a(n) =
 *            101*10^(n-2) + (10^(n-3)-1)/9 for n > 2 a(0) = 1, a(1) = 10,
 *            a(2) = 101
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)101;

    /* n >= 3 */
    idx_t k = n - 3;          /* exponent for 10^(n-3) */
    seq_t p = pow10(k);       /* 10^(n-3)           */
    seq_t t = pow10(k + 1);   /* 10^(n-2)           */

    seq_t part1 = (seq_t)101 * t;          /* 101*10^(n-2) = 1010*10^(n-3) */
    seq_t part2 = (p - 1) / (seq_t)9;     /* (10^(n-3)-1)/9           */

    return part1 + part2;
}
