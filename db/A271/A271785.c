/*
 * A271785  a(n) = Sum_{k=0..(n-1)/2} (n+2-k)*binomial(n-1-k,k).
 * https://oeis.org/A271785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+a(n-2)-2*a(n-3)-a(n-4) Initial values: a(0)=0,
 *            a(1)=3, a(2)=4, a(3)=9
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 40, u64 n <= 85, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)9;

    /* Working variables hold a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a0 = (seq_t)0;   /* a(i-4) */
    seq_t a1 = (seq_t)3;   /* a(i-3) */
    seq_t a2 = (seq_t)4;   /* a(i-2) */
    seq_t a3 = (seq_t)9;   /* a(i-1) */
    seq_t ai;              /* a(i) */

    for (idx_t i = 4; i <= n; ++i) {
        /* ai = 2*a3 + a2 - 2*a1 - a0 */
        seq_t t = a3;          /* copy a3 */
        t += a3;               /* 2*a3 */
        t += a2;               /* + a2 */
        seq_t s = a1;          /* copy a1 */
        s += a1;               /* 2*a1 */
        s += a0;               /* + a0 */
        ai = t - s;            /* result */

        /* shift the window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;   /* a(n) */
}
