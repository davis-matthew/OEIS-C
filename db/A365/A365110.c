/*
 * A365110  G.f. satisfies A(x) = 1 + x / (1 + x*A(x))^3.
 * https://oeis.org/A365110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n}
 *            (-1)^(n-k)*binomial(n-k+1,k)*binomial(n+2*k-1,n-k)/(n-k+1)
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 15, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t result = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t nmk = n - k;                                 /* n - k */
        seq_t c1 = binomial(nmk + 1, k);                   /* binomial(n-k+1, k) */
        if (c1 == 0) continue;                             /* skip zero terms */
        seq_t c2 = binomial(n + 2 * k - 1, nmk);           /* binomial(n+2k-1, n-k) */
        seq_t term = c1 * c2;
        if ((nmk & 1u) != 0) term = -term;                 /* (-1)^(n-k) */
        seq_t denom = nmk + 1;
        term = term / denom;                               /* divide by (n-k+1) */
        result += term;
    }
    return result;
}
