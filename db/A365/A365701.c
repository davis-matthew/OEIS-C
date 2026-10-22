/*
 * A365701  G.f. satisfies A(x) = 1 + x^5*A(x)^4 / (1 - x*A(x)).
 * https://oeis.org/A365701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/5)} binomial(n-4k-1, n-5k) * *
 *            binomial(n-k+1, k) / (n-k+1)
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 39, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* handle the offset term a(0) explicitly */
    if (n == 0) return (seq_t)1;

    seq_t result = 0;
    idx_t k_limit = n / 5;
    for (idx_t k = 0; k <= k_limit; ++k) {
        /* indices for the first binomial */
        idx_t m1 = n - 4 * k - 1;   /* n-4k-1 */
        idx_t m2 = n - 5 * k;       /* n-5k */

        /* skip terms where the binomial is zero (except possibly m1 == -1 && m2 == 0) */
        if (m1 < m2) continue;
        if (m1 < 0) continue; /* m1 can only be -1 when n==0, but that case handled above */

        /* indices for the second binomial */
        idx_t m3 = n - k + 1;      /* n-k+1 */
        idx_t m4 = k;              /* k */

        if (m3 < m4) continue;
        if (m3 < 0) continue; /* should not happen for n>0 */

        seq_t b1 = binomial(m1, m2);
        seq_t b2 = binomial(m3, m4);

        /* compute (b1 * b2) / (n - k + 1) */
        seq_t prod = b1 * b2;
        prod /= (seq_t)(n - k + 1);

        result += prod;
    }
    return result;
}
