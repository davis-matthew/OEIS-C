/*
 * A279013  a(n) = Sum_{k=0..n} binomial(2*k,k)/(k+1)*binomial(2*n-1,n-k).
 * https://oeis.org/A279013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*k,k)/(k+1) * binomial(2*n-1,n-k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return 1;

    /* initialise Catalan(0) = 1 and B(0) = binomial(2*n-1,n) */
    seq_t C = 1;
    seq_t B = binomial(2 * n - 1, n);
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        sum += C * B;

        if (k == n) /* last iteration */
            break;

        /* next Catalan: C_{k+1} = C_k * (4k+2)/(k+2) */
        seq_t num = (seq_t)(4 * k + 2);
        seq_t den = (seq_t)(k + 2);
        C = C * num / den;

        /* next B: B_{k+1} = binomial(2*n-1, (n-k)-1)
           recurrence: binom(N,r-1) = binom(N,r) * r/(N-r+1)   */
        seq_t r   = (seq_t)(n - k);          /* current r = n-k  */
        seq_t N   = (seq_t)(2 * n - 1);      /* N = 2n-1       */
        seq_t denB = (seq_t)(N - r + 1);      /* N-r+1         */
        B = B * r / denB;
    }

    return sum;
}
