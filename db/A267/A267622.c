/*
 * A267622  Binary representation of the n-th iteration of the "Rule 187"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 * a(n) = 10^(2n) + (10^(2n-1) - 1) / 9 for n >= 1 *
 *            (the digits of a(n) are “10” followed by (2n‑1) ones)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t exp_high = 2 * n;        /* 2n  */
    idx_t exp_low  = exp_high - 1; /* 2n-1 */

    seq_t high = pow10(exp_high);   /* 10^(2n)          */
    seq_t low  = pow10(exp_low);    /* 10^(2n-1)        */

    /* (10^(2n-1) - 1)/9 gives the decimal number consisting of (2n-1) ones */
    seq_t ones = (low - (seq_t)1) / (seq_t)9;

    return high + ones;
}
