/*
 * A367281  G.f. satisfies A(x) = 1 + x*A(x)^2 / (1 - x*A(x)^3)^3.
 * https://oeis.org/A367281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(t*k+u*(n-k)+1,k) *
 *            binomial(n+(s-1)*k-1,n-k) / (t*k+u*(n-k)+1) with t=2, u=3, s=3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t denom = 3 * n - k + 1;          /* t*k + u*(n-k) + 1 = 3*n - k + 1 */
        seq_t bin1 = binomial(3 * n - k + 1, k);
        seq_t bin2 = binomial(n + 2 * k - 1, n - k);
        seq_t term = (bin1 * bin2) / denom;
        sum += term;
    }
    return sum;
}
