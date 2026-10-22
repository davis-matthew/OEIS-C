/*
 * A363754  a(n) = Sum_{k=0..n} F(2k-1)*F(2k)*F(2k+1)/2, where F(n) is the
 *          Fibonacci number A000045(n).
 * https://oeis.org/A363754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (F(2n+1)^3 + F(2n+1) - 2)/8 or equivalently a(0)=0,
 *            a(1)=1, a(2)=16, a(3)=276, a(4)=4917, a(5)=88132, a(n) =
 *            22*a(n-1) - 77*a(n-2) + 77*a(n-3) - 22*a(n-4) + a(n-5) for all n
 *            ≥ 6. Uses only integer arithmetic on seq_t
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)16;
    if (n == 3) return (seq_t)276;
    if (n == 4) return (seq_t)4917;
    if (n == 5) return (seq_t)88132;

    /* Use a rolling window of the last five terms */
    seq_t a1 = (seq_t)1;      /* a(n-5)  when n=6   */
    seq_t a2 = (seq_t)16;     /* a(n-4)  when n=6   */
    seq_t a3 = (seq_t)276;    /* a(n-3)  when n=6   */
    seq_t a4 = (seq_t)4917;   /* a(n-2)  when n=6   */
    seq_t a5 = (seq_t)88132;  /* a(n-1)  when n=6   */
    seq_t an = (seq_t)0;

    for (idx_t i = 6; i <= n; ++i)
    {
        an = 22 * a5 - 77 * a4 + 77 * a3 - 22 * a2 + a1;
        /* Shift the window for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }

    return an;
}
