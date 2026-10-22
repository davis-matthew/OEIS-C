/*
 * A397508  a(n) = Sum_{k=0..n} (binomial(5*n+2*k-3,k) -
 *          binomial(5*n+2*k-3,k-1)).
 * https://oeis.org/A397508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} (C(5*n+2*k-3, k) - C(5*n+2*k-3, k-1)) * 1
 *            where C(N,K)=0 for K<0 or K>N
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Use an intermediate 128‑bit type to compute binomial coefficients
    exactly for the values that fit in the required long‑term limits.  */

#include <stdint.h>

/* Compute C(N,K) with a 128‑bit intermediate to avoid overflow.  */
static seq_t binom128(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K == 0 || K == N) return 1;
    if (K > N - K) K = N - K;      /* use symmetry */

    __int128 res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        res = res * ( (__int128)(N - K + i) ) / i;
    }
    return (seq_t)res;             /* cast to seq_t – safe for the sizes used */
}

seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        idx_t N = 5*n + 2*k - 3;   /* N >= -3 for n==0, handled below */
        seq_t term1 = binom128(N, k);
        seq_t term2 = (k > 0) ? binom128(N, k-1) : 0;
        sum += term1 - term2;
    }

    return sum;
}
