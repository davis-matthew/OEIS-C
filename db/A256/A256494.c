/*
 * A256494  Expansion of -x^2*(x^3+x-1) / ((x-1)*(x+1)*(2*x-1)*(x^2+1)).
 * https://oeis.org/A256494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-4) - 2*a(n-5) with initial values a(1)=0,
 *            a(2)=1, a(3)=1, a(4)=2, a(5)=3
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Handle the initial five terms explicitly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)1;
    if (n == 4) return (seq_t)2;
    if (n == 5) return (seq_t)3;

    /* Iterate from 6 up to n, keeping the last five terms
       in a small circular buffer. */
    seq_t a1 = (seq_t)0;  /* a(n-5) */
    seq_t a2 = (seq_t)1;  /* a(n-4) */
    seq_t a3 = (seq_t)1;  /* a(n-3) */
    seq_t a4 = (seq_t)2;  /* a(n-2) */
    seq_t a5 = (seq_t)3;  /* a(n-1) */
    seq_t an;              /* current a(n) */

    for (idx_t i = 6; i <= n; ++i) {
        an = 2 * a5 + a2 - 2 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }
    return an;
}
