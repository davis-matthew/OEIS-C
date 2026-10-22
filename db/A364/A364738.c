/*
 * A364738  G.f. satisfies A(x) = 1 + x*A(x) / (1 + x*A(x)^5).
 * https://oeis.org/A364738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/n) * Σ_{k=0}^{n-1} (-1)^k * binomial(n,k) *
 *            binomial(n+4*k, n-1-k) for n>0; a(0)=1
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 15, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>                       /* required by the build */
#include "../utils/binomial.h"             /* provides binomial(idx_t,a,b) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                         /* a(0) = 1 */

    seq_t sum = 0;
    idx_t k;
    for (k = 0; k < n; ++k) {
        seq_t term = binomial(n, k) * binomial(n + 4 * k, n - 1 - k);
        if (k & 1)                          /* (-1)^k */
            term = -term;
        sum += term;
    }
    return sum / (seq_t)n;                  /* integer division */
}
