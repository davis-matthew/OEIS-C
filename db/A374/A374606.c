/*
 * A374606  a(n) = Sum_{k = 0..n} binomial(n, k)^2*binomial(n+k,
 *          k)*binomial(3*n+2*k+1, n).
 * https://oeis.org/A374606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..n} binomial(n,k)^2 * binomial(n+k,k) *
 *            binomial(3*n+2*k+1 , n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* special case n = 0 */
    if (n == 0)
        return 1;

    /* sum of terms over k */
    seq_t sum = 0;

    /* C(n,k) starting at k=0 */
    seq_t Cnk = 1;                /* binomial(n,0) */
    /* C(n+k,k) starting at k=0 */
    seq_t Cnk2 = 1;               /* binomial(n+0,0) */

    /* binomial(3*n+1, n) = binom(3*n+1, n) */
    idx_t top_first = 3 * n + 1;
    seq_t C1 = binom(top_first, n);

    /* N = 3*n + 2*k + 1, initial N when k = 0 */
    seq_t N = (seq_t)top_first;

    for (idx_t k = 0; k <= n; ++k) {
        /* term = Cnk^2 * Cnk2 * C1 */
        seq_t term = Cnk;
        term = term * Cnk;          /* Cnk^2 */
        term = term * Cnk2;         /* * C(n+k,k) */
        term = term * C1;           /* * C(3*n+2*k+1 , n) */
        sum += term;

        if (k == n)
            break;                  /* last iteration finished */

        /* update Cnk to C(n,k+1) = Cnk * (n-k)/(k+1) */
        Cnk = Cnk * (seq_t)(n - k);
        Cnk /= (k + 1);

        /* update Cnk2 to C(n+k+1,k+1) = Cnk2 * (n+k+1)/(k+1) */
        Cnk2 = Cnk2 * (seq_t)(n + k + 1);
        Cnk2 /= (k + 1);

        /* update C1 to C(3*n+2*(k+1)+1 , n) */
        /* N1 = N+1, N2 = N+2, d1 = N1 - n, d2 = N2 - n */
        seq_t N1 = N + 1;
        seq_t N2 = N + 2;
        seq_t d1 = N1 - n;          /* N+1 - n */
        seq_t d2 = N2 - n;          /* N+2 - n */

        C1 = C1 * N1;
        C1 /= d1;
        C1 = C1 * N2;
        C1 /= d2;

        N = N2;                     /* next N = 3*n + 2*(k+1) + 1 */
    }

    return sum;
}
