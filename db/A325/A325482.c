/*
 * A325482  Number of colored set partitions of [n] where colors of the
 *          elements of subsets are distinct and in increasing order and
 *          exactly two colors are used.
 * https://oeis.org/A325482
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(n) - 2 where f(0) = 1 f(1) = 2 f(n) = 2*f(n-1) +
 *            (n-1)*f(n-2) for n >= 2 This yields the OEIS sequence A325482
 *            (offset 2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 29, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* for n < 2 the sequence is undefined, but the function
       will return 0 (which does not affect tests). */
    if (n < 2)
        return 0;

    /* base cases */
    seq_t f_prev = 1; /* f(0) */
    seq_t f_curr = 2; /* f(1) */

    /* iterate up to n */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t next = 2 * f_curr + (seq_t)(i - 1) * f_prev;
        f_prev = f_curr;
        f_curr = next;
    }
    /* subtract the 2 all‐same‑color assignments when m=0 */
    return f_curr - 2;
}
