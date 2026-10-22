/*
 * A371379  G.f. A(x) satisfies A(x) = (1 + x*A(x)^(3/2) / (1-x))^4.
 * https://oeis.org/A371379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*Σ_{k=0..n} binomial(n-1,n-k)*binomial(6*k+3,k)/(5*k+4)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0.  a(0)=1. */

#include <stdint.h>

static inline seq_t binom_fixed(idx_t n, idx_t k) {
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* use __int128 to avoid overflow before division */
        __int128 res = (__int128)result * (n - k + i);
        res /= i;
        result = (seq_t)res;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t bin1 = binom_fixed(n - 1, k - 1);

        idx_t N = 6 * k + 3;
        idx_t K = k;
        __int128 bin2 = 1;
        for (idx_t i = 1; i <= K; ++i) {
            bin2 = bin2 * (N - K + i);
            bin2 /= i;
        }

        __int128 term = (__int128)bin1 * bin2 * 4;
        term /= (5 * k + 4);
        sum += (seq_t)term;
    }
    return sum;
}
