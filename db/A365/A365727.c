/*
 * A365727  G.f. satisfies A(x) = 1 + x^4*A(x)*(1 + x*A(x)).
 * https://oeis.org/A365727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binomial(k,n-4k) *
 *            binomial(n-3k+1,k) / (n-3k+1)
 * Range:     u8 n <= 18, u16 n <= 38, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t k = 0; 4*k <= n; ++k)
    {
        idx_t t1 = n - 4*k;                   /* upper binomial argument (may exceed k) */
        seq_t b1 = binomial(k, t1);           /* binomial(k, n-4k) */
        seq_t b2 = binomial(n - 3*k + 1, k);  /* binomial(n-3k+1, k) */
        seq_t denom = (seq_t)(n - 3*k + 1);
        seq_t term = (b1 * b2) / denom;       /* integer division, exact by definition */
        res += term;
    }
    return res;
}
