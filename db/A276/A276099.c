/*
 * A276099  a(n) = (9*n)!*(5/2*n)!/((9*n/2)!*(5*n)!*(2*n)!).
 * https://oeis.org/A276099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(14*n, 2*n-2*k)*binomial(5*n+k-1, k)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 explicitly */
    if (n == 0) {
        return 1;
    }

    /* Pre‑compute constants */
    idx_t N = 14 * n;        /* upper limit for the first binomial   */
    idx_t m = 2 * n;         /* current lower argument for B(k)       */

    /* Initial binomial values */
    seq_t B = binomial(N, m);   /* B(0) = binom(14*n, 2*n)              */
    seq_t A = 1;                /* A(0) = binom(5*n-1, 0) = 1           */

    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* Add the current term */
        sum += B * A;

        /* Update B and A for the next k, unless we are at the last iteration */
        if (k == n) {
            break;
        }

        /* Update B: B(k+1) = B(k) * [m*(m-1)] / [(N-m+1)*(N-m+2)]   */
        seq_t numB = (seq_t)m * (seq_t)(m - 1);
        seq_t denB = (seq_t)(N - m + 1) * (seq_t)(N - m + 2);
        B = B * numB / denB;
        m -= 2;        /* next lower index for B */

        /* Update A: A(k+1) = A(k) * (5*n + k) / (k+1)                */
        seq_t numA = (seq_t)(5 * n + k);
        seq_t denA = (seq_t)(k + 1);
        A = A * numA / denA;
    }

    return sum;
}
