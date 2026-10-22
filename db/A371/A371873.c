/*
 * A371873  a(n) = Sum_{k=0..floor(n/3)} binomial(2*n+1,n-3*k).
 * https://oeis.org/A371873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(2*n+1, n-3*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* N = 2*n+1, m = n, bin = C(N, m) */
    idx_t Nidx = 2 * n + 1;
    seq_t N   = (seq_t)Nidx;
    seq_t m   = (seq_t)n;
    seq_t bin = binomial(Nidx, (idx_t)m);
    seq_t sum = 0;

    while (1) {
        sum += bin;
        if (m < 3) break;

        /* compute next binomial for m-3
           C(N, m-3) = C(N, m) * m*(m-1)*(m-2) / ((N-m+1)*(N-m+2)*(N-m+3))
        */
        seq_t a1 = m;
        seq_t b1 = m - 1;
        seq_t c1 = m - 2;
        seq_t d1 = N - m + 1;
        seq_t e1 = N - m + 2;
        seq_t f1 = N - m + 3;

        bin = bin * a1 / d1;
        bin = bin * b1 / e1;
        bin = bin * c1 / f1;
        m   = m - 3;
    }

    return sum;
}
