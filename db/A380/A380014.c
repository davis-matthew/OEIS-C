/*
 * A380014  Expansion of e.g.f. 1/sqrt(exp(-2*x) - 2*x).
 * https://oeis.org/A380014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binom(n,k) * (2k-1)!! * (2k+1)^(n-k) where
 *            (2k-1)!! = product of odd numbers up to (2k-1) and binom(n,k) =
 *            n!/(k!(n-k)!) The implementation computes the sum directly in
 *            nested loops, avoiding binomial factorization and exponentiation
 *            by using the recurrence f_k(n+1) = f_k(n) * (...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate arrays for odd double factorials and result terms */
    size_t sz = (size_t)n + 1;
    seq_t *odf = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *res = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!odf || !res) {
        free(odf);
        free(res);
        return (seq_t)0;
    }

    /* (2k-1)!! initialization : (-1)!! = 1, 1!! = 1, 3!! = 3, ... */
    odf[0] = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        odf[k] = odf[k - 1] * (seq_t)(2 * k - 1);
    }

    /* Clear result array */
    for (size_t i = 0; i < sz; ++i) res[i] = (seq_t)0;

    /* Main double loop */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t val = (seq_t)1;           /* f_k(k) = 1 */
        for (idx_t m = k; m <= n; ++m) {
            if (m > k) {
                /* val = val * (m * (2k+1)) / (m - k) */
                seq_t factor = (seq_t)m * (seq_t)(2 * k + 1);
                val = val * factor / (seq_t)(m - k);
            }
            res[m] += odf[k] * val;
        }
    }

    seq_t answer = res[n];
    free(res);
    free(odf);
    return answer;
}
