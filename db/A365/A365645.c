/*
 * A365645  a(n) = n*(1 + n)*(10^n - 1)/18.
 * https://oeis.org/A365645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(10^n - 1)/18
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute n*(n+1)/2 first (exact division by 2) */
    seq_t nn = (seq_t)n;
    seq_t a_part = nn * (nn + 1) / 2;

    /* Compute (10^n - 1)/9 (exact division by 9) */
    seq_t p10 = pow10(n);            /* arbitrary‑precision 10^n */
    seq_t b_part = (p10 - 1) / 9;

    /* Final product gives the desired term */
    return a_part * b_part;
}
