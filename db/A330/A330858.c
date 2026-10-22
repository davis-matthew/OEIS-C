/*
 * A330858  Triangle read by rows: T(n,k) is the number of permutations in S_n
 *          for which all cycles have length <= k.
 * https://oeis.org/A330858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) reads the triangle T(n,k) row‑wise. T(n,k) is the number of
 *            permutations of {1,…,n} all of whose cycle lengths are ≤ k. For
 *            each n we determine the row m and column k such that (m−1)m/2 <
 *            n ≤ m(m+1)/2 and return T(m,k). The recurrence used is T(0,k) =
 *            1 T(n,k) = Σ_{j=1}^{min(k,n)} (n−1 choos...
 * Range:     u8 n <= 17, u16 n <= 39, u32 n <= 87, u64 n <= 220,
 *            u128 n <= 605, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* external linkage as required */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined, but return 0 safely */

    /* Find row m such that S_{m-1} < n ≤ S_m, with S_m = m(m+1)/2 */
    idx_t m = 0, sum = 0;
    while (sum < n) {
        ++m;
        sum += m;                /* now sum = S_m */
    }
    idx_t prev_sum = sum - m;    /* S_{m-1} */
    idx_t k = n - prev_sum;      /* column index in row m (1‑based) */

    /* Allocate temporary arrays for DP and factorials */
    seq_t *dp   = (seq_t *) calloc((size_t)(m + 1), sizeof(seq_t));
    seq_t *fact = (seq_t *) calloc((size_t)(m + 1), sizeof(seq_t));
    if (!dp || !fact) {          /* allocation failed – return 0 */
        free(dp);
        free(fact);
        return 0;
    }

    dp[0]   = 1;
    fact[0] = 1;
    for (idx_t i = 1; i <= m; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    for (idx_t i = 1; i <= m; ++i) {
        seq_t sumdp = 0;
        idx_t maxj = (k < i) ? k : i;
        for (idx_t j = 1; j <= maxj; ++j) {
            seq_t coeff = fact[i - 1] / fact[i - j];
            sumdp += coeff * dp[i - j];
        }
        dp[i] = sumdp;
    }

    seq_t result = dp[m];
    free(dp);
    free(fact);
    return result;
}
