/*
 * A351770  a(n) = Sum_{j=1..n} Sum_{i=1..j} (i*j)^5.
 * https://oeis.org/A351770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..n} Sum_{i=1..j} (i*j)^5
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 8, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* Helper: compute x^5 for seq_t values */
static seq_t pow5(seq_t x)
{
    seq_t res = x * x;      /* x^2 */
    res *= res;             /* x^4 */
    res *= x;               /* x^5 */
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Compute S_n = sum_{k=1..n} k^5 */
    seq_t n_t   = (seq_t)n;
    seq_t n1    = n_t + (seq_t)1;                /* n + 1 */
    seq_t n2    = n_t * n_t;                    /* n^2 */
    seq_t n1_2  = n1 * n1;                      /* (n + 1)^2 */
    seq_t two_n = (seq_t)2 * n_t;                /* 2n */
    seq_t two_n2 = (seq_t)2 * n2;                /* 2n^2 */
    seq_t inner = two_n2 + two_n - (seq_t)1;    /* 2n^2 + 2n - 1 */
    seq_t Sn = n2 * n1_2 * inner;               /* numerator of S_n */
    Sn /= (seq_t)12;                            /* division by 12 */

    seq_t res = (seq_t)0;
    seq_t S_prev = (seq_t)0;

    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        seq_t i_t  = (seq_t)i;
        seq_t i5   = pow5(i_t);
        res += i5 * (Sn - S_prev);
        S_prev += i5;
    }

    return res;
}
