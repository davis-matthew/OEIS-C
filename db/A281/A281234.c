/*
 * A281234  Solutions y to the negative Pell equation y^2 = 72*x^2 - 288 with
 *          x,y >= 0.
 * https://oeis.org/A281234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2) for n>2, a(1)=0, a(2)=48
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0U;
    if (n == 2) return (seq_t)48U;

    seq_t prev2 = (seq_t)0U;          /* a(1)   */
    seq_t prev1 = (seq_t)48U;         /* a(2)   */
    seq_t cur   = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        /* Compute 5*prev1 without overflow by repeated addition */
        seq_t five_times = prev1 + prev1 + prev1 + prev1 + prev1; /* 5*prev1 */
        /* Compute prev1 - prev2 (non‑negative because a is increasing) */
        seq_t diff = prev1 - prev2;
        cur = five_times + diff;          /* a(i) = 5*a(i-1) + (a(i-1)-a(i-2)) */
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
