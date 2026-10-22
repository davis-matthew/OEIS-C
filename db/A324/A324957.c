/*
 * A324957  a(n) is the coefficient of y^(n-1) in Product_{k=0..n-2} (n + (n +
 *          k)*y + n*y^2), for n >= 1.
 * https://oeis.org/A324957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{r=0}^{⌊(n-1)/2⌋} C(2r, r) · n^{2r} ·
 *            e_{\,n-1-2r}(n,n+1,…,2n-2), where e_s is the elementary
 *            symmetric sum of degree s of the set {n, n+1, …, 2n-2}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* n must be ≥ 1; for n = 0 return 0 as a safe default. */
    if (n == 0) return 0;

    idx_t M = n - 1;                 /* number of factors in the product */

    /* Elementary symmetric sums e[0..M] of the numbers {n, n+1, …, 2n-2} */
    static seq_t e[20000];           /* supports n up to ~10000 safely   */
    for (idx_t i = 0; i <= M; ++i) e[i] = 0;
    e[0] = 1;

    /* Build the symmetric sums by iterating over the set. */
    for (idx_t i = 0; i < M; ++i) {
        seq_t val = (seq_t)n + (seq_t)i;            /* n + i   */
        for (idx_t s = i + 1; s > 0; --s) {
            e[s] = e[s] + e[s - 1] * val;
        }
    }

    /* Accumulate the final coefficient. */
    seq_t result = 0;
    for (idx_t r = 0; 2 * r <= M; ++r) {
        idx_t b = M - 2 * r;                     /* s = n-1-2r   */
        seq_t comb = binomial(2 * r, r);         /* C(2r, r)     */
        seq_t npow = seq_pow((seq_t)n, 2 * r);   /* n^{2r}       */
        seq_t term = comb * npow * e[b];
        result = result + term;
    }
    return result;
}
