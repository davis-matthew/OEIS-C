/*
 * A279127  a(n) = Sum_{0<=m<n} Product_{-m<=j<=m} (n-j).
 * https://oeis.org/A279127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A279127: a(n) = Sum_{0<=m<n} Product_{-m<=j<=m} (n-j)
   Recurrence used (valid for n≥0):
     a(0)=0, a(1)=1, a(2)=8, a(3)=147
     a(n+3) = -a(n) + (4*n^2+6*n-1)*a(n+1) + (4*n^2+18*n+17)*a(n+2) + 8*n+12
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)147;

    /* Sliding window for the recurrence */
    seq_t w[4] = {(seq_t)0, (seq_t)1, (seq_t)8, (seq_t)147};

    for (idx_t k = 1; k <= n - 3; ++k)
    {
        /* Compute coefficients 4*k^2 + 18*k + 17  and  4*k^2 + 6*k - 1  */
        idx_t kk   = k * k;
        idx_t c1   = 4 * kk + 18 * k + 17;   /* coefficient for a(k+2) */
        idx_t c2   = 4 * kk + 6 * k  - 1;    /* coefficient for a(k+1) */

        seq_t term = (seq_t)c1 * w[3]          /* c1 * a(k+2) */
                  + (seq_t)c2 * w[2]          /* c2 * a(k+1) */
                  + (seq_t)(8 * k + 12)      /* + 8*k + 12   */
                  - w[1];                    /* - a(k)       */

        /* Slide the window: w[0], w[1], w[2] move one step forward */
        w[0] = w[1];
        w[1] = w[2];
        w[2] = w[3];
        w[3] = term;
    }

    return w[3];   /* w[3] now holds a(n) */
}
