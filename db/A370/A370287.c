/*
 * A370287  Coefficient of x^n in the expansion of ( (1+x)^3 + x^3 )^n.
 * https://oeis.org/A370287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..floor(n/3)} binomial(n,k) * binomial(3*n-3*k,
 *            n-3*k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Coefficient of x^n in ((1+x)^3 + x^3)^n) */

#include <stdint.h>

/* multiplicative binomial, result fits in seq_t (the build type) */
static seq_t binom_int(idx_t N, idx_t K)
{
    if (K > N - K)
        K = N - K;
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i) {
        res = res * (seq_t)(N - K + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t m = n / 3;                       /* floor(n/3) */
    seq_t ans = 0;
    seq_t comb_nk = 1;                     /* binomial(n,0) */

    for (idx_t k = 0; k <= m; ++k) {
        seq_t bin2 = binom_int(3 * n - 3 * k, n - 3 * k);
        ans += comb_nk * bin2;
        if (k < m) {
            /* Update binomial(n,k+1) = binomial(n,k)*(n-k)/(k+1) */
            comb_nk = comb_nk * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }
    return ans;
}
