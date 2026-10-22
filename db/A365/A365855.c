/*
 * A365855  Expansion of (1/x) * Series_Reversion( x*(1+x)^2*(1-x)^4 ).
 * https://oeis.org/A365855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/2⌋} binomial(2*n+k+1,k) *
 *            binomial(3*n-2*k+1,n-2*k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t kmax = n >> 1;               /* floor(n/2) */
    unsigned __int128 sum = 0;

    /* k = 0 term */
    unsigned __int128 A = 1;           /* binom(2*n+1,0)          */
    unsigned __int128 B = (unsigned __int128)binomial(3*n + 1, n);  /* binom(3*n+1,n)   */
    sum += A * B;

    for (idx_t k = 0; k < kmax; ++k)
    {
        /* A_{k+1} = A_k * (2*n + k + 2) / (k + 1) */
        unsigned __int128 numA = (unsigned __int128)(2*n + k + 2);
        unsigned __int128 denA = (unsigned __int128)(k + 1);
        A = (A * numA) / denA;

        /* B_{k+1} = B_k * (n-2*k)(n-2*k-1) / ((3*n-2*k+1)(3*n-2*k)) */
        unsigned __int128 numB = (unsigned __int128)(n - 2*k);
        numB *= (unsigned __int128)(n - 2*k - 1);
        unsigned __int128 denB = (unsigned __int128)(3*n - 2*k + 1);
        denB *= (unsigned __int128)(3*n - 2*k);
        B = (B * numB) / denB;

        sum += A * B;
    }

    sum /= (unsigned __int128)(n + 1);
    return (seq_t)sum;
}
