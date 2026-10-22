/*
 * A378495  Triangle read by rows: T(n,k) is the number of derangements in S_n
 *          with no k-cycles. 1 <= k <= n.
 * https://oeis.org/A378495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,col) where * r = smallest integer with r(r+1)/2 ≥
 *            n, k = n - (r-1)r/2. * T(n,k) = 0 (k = 1) * T(n,k) = Σ_{i=0..n}
 *            (-1)^i * binomial(n,i) * A122974(n-i,k) (k > 1) * * A122974(m,k)
 *            = Σ_{j=0..⌊m/k⌋} (-1)^j * m! / (k^j * j!) * * All operations are
 *            performed in the integer type seq_t
 * Range:     u8 n <= 16, u16 n <= 37, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n < 1 is outside the offset */

    /* Find row r and column k from the index n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;          /* r(r+1)/2  >= n */

    idx_t row_start = (r - 1) * r / 2;          /* first position of row r */
    idx_t k = n - row_start;                    /* column in row r (1‑based) */

    if (k == 1) return 0;                      /* T(n,1) = 0 */

    /* Allocate factorials up to r (r ≤ 141 for n ≤ 10000) */
    seq_t *fact = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!fact) return 0;                       /* allocation failed */

    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= r; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    seq_t total = (seq_t)0;                     /* accumulator for T(r,k) */
    seq_t binom = (seq_t)1;                     /* binomial(r,i)  (i=0) */

    for (idx_t i = 0; i <= r; ++i) {
        idx_t m = r - i;                         /* argument for A122974 */

        /* Compute A122974(m,k) */
        idx_t maxj = m / k;
        seq_t sum2 = (seq_t)0;
        seq_t pow_k = (seq_t)1;     /* k^j */
        seq_t fact_j = (seq_t)1;    /* j! */
        for (idx_t j = 0; j <= maxj; ++j) {
            if (j > 0) {
                pow_k *= (seq_t)k;
                fact_j *= (seq_t)j;
            }
            seq_t denom = pow_k * fact_j;
            seq_t term  = fact[m] / denom;      /* exact division */
            if (j & 1) sum2 -= term;
            else        sum2 += term;
        }

        seq_t prod = binom * sum2;
        if (i & 1) total -= prod;
        else       total += prod;

        /* Update binomial to C(r,i+1) for next iteration */
        if (i < r) {
            binom *= (seq_t)(r - i);
            binom /= (seq_t)(i + 1);
        }
    }

    free(fact);
    return total;
}
