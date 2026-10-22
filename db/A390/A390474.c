/*
 * A390474  a(n) = Sum_{k=0..n} binomial(4*n+3*k+2,n-k).
 * https://oeis.org/A390474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*n+3*k+2, n-k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

static seq_t binom_multiplicative(idx_t N, idx_t K)
{
    /* compute C(N,K) = N!/(K!(N-K)!) multiplicatively,
       avoiding intermediate overflow by cancelling factors. */
    if (K > N - K)
        K = N - K;
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i)
    {
        seq_t num = (seq_t)(N - K + i);
        seq_t i_seq = (seq_t)i;
        seq_t g = gcd(res, i_seq);   /* cancel common factor */
        res /= g;
        i_seq /= g;
        /* i_seq now divides (res * num) exactly */
        res = res * num / i_seq;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        idx_t N = 4 * n + 3 * k + 2;   /* 4n+3k+2 */
        idx_t K = n - k;               /* n-k  */
        total += binom_multiplicative(N, K);
    }
    return total;
}
