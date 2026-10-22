/*
 * A334066  a(n) = (2n-1)!!*(Sum_{k=1..n}k/(2*k-1)).
 * https://oeis.org/A334066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n-1)!! * (Sum_{k=1..n} k/(2k-1))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=5, a(n)=2*a(n-1)+(2n-3)^2*a(n-2)+(2n-1)!! for n>=3 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;

    seq_t a_prevprev = (seq_t)1;          /* a(1) */
    seq_t a_prev     = (seq_t)5;          /* a(2) */
    seq_t odd_fact   = (seq_t)3;          /* (2*2-1)!! */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t t = (seq_t)(2 * i - 3);     /* 2i-3 */
        odd_fact = odd_fact * ((seq_t)(2 * i - 1));   /* (2i-1)!! */

        seq_t a_curr = (seq_t)2 * a_prev + t * t * a_prevprev + odd_fact;

        a_prevprev = a_prev;
        a_prev     = a_curr;
    }
    return a_prev;
}
