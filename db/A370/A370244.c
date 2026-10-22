/*
 * A370244  Coefficient of x^n in the expansion of ( 1/(1-x) * (1+x^2)^3 )^n.
 * https://oeis.org/A370244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(3*n,k) * binomial(2*n-2*k-1,
 *            n-2*k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom_multiply
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A370244 */

#include <stdbool.h>
#include "../utils/binom_multiply.h"

/* auxiliary: compute binomial(N,K) multiplicatively with arbitrary-precision seq_t */

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return 1;

    idx_t maxk = n / 2;

    /* C3 = binomial(3*n, k) – start with k = 0 */
    seq_t C3 = 1;              /* binomial(3*n,0) = 1 */

    /* B = binomial(2*n-1, n) – the k = 0 term of the second binomial */
    seq_t B = binom_multiply(2*n - 1, n);

    /* r and m are the parameters of the current second binomial:
       B = binomial(m, r) with m = 2*n - 1 - 2*k, r = n - 2*k */
    seq_t r = n;        /* r for k = 0 */
    seq_t m = 2*n - 1;  /* m for k = 0 */

    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k)
    {
        /* add current term to sum */
        sum += C3 * B;

        if (k == maxk) break;   /* last term processed */

        /* update C3 to binomial(3*n, k+1) */
        seq_t numC3 = (seq_t)(3*n - k);   /* (3n - k) */
        seq_t denC3 = (seq_t)(k + 1);     /* (k+1) */
        C3 = (C3 * numC3) / denC3;

        /* update B to binomial(m-2, r-2) */
        /* B = B * r*(r-1) / (m*(m-1)) */
        seq_t numB = r * (r - 1);
        seq_t denB = m * (m - 1);
        B = (B * numB) / denB;

        /* prepare r and m for next iteration */
        r -= 2;
        m -= 2;
    }

    return sum;
}
