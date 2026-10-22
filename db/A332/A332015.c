/*
 * A332015  Number of compositions (ordered partitions) of n into distinct
 *          heptagonal numbers.
 * https://oeis.org/A332015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{S⊆H, sum(S)=n} |S|! (H = set of distinct heptagonal
 *            numbers) Algorithm: DP over the heptagonal numbers. For each sum
 *            s and subset size k we store the number of subsets of size k
 *            whose elements sum to s. Finally we multiply by k! for each k
 *            and sum
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* helper: factorials up to m */
static void compute_factorials(size_t m, seq_t *fac) {
    fac[0] = 1;
    for (size_t i = 1; i <= m; ++i)
        fac[i] = fac[i - 1] * (seq_t)i;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* Offset 0 has the empty composition */
    if (n == 0)
        return 1;

    /* Build list of heptagonal numbers ≤ n.
       H(k) = k*(5k-3)/2,  k >= 1.  At most ~63 values for n ≤ 10000. */
    idx_t hept[128];
    size_t hcnt = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t hk = (seq_t)k * ((seq_t)5 * k - 3) / 2;
        if (hk > (seq_t)n)
            break;
        hept[hcnt++] = (idx_t)hk;
    }

    size_t cols = hcnt + 1;                 /* second dimension for subset size */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1) * cols, sizeof(seq_t));
    if (!dp)                                 /* out‑of‑memory protection */
        return 0;
    dp[0] = 1;                               /* dp[0][0] = 1 */

    /* DP over heptagonal numbers */
    for (size_t i = 0; i < hcnt; ++i) {
        idx_t val_idx = hept[i];
        size_t val    = (size_t)val_idx;     /* for index arithmetic */
        size_t maxk   = i + 1;               /* maximum subset size after this element */

        for (idx_t s = n; ; --s) {
            if (s < val_idx) break;
            for (size_t k = maxk; k > 0; --k) {
                /* Add subsets that include the current value */
                dp[(size_t)s * cols + k] +=
                    dp[(size_t)(s - val_idx) * cols + (k - 1)];
            }
            if (s == 0) break;               /* safety, though will not happen */
        }
    }

    /* Pre‑compute factorials up to hcnt */
    seq_t *fac = (seq_t *)malloc((hcnt + 1) * sizeof(seq_t));
    if (!fac) {
        free(dp);
        return 0;
    }
    compute_factorials(hcnt, fac);

    /* Sum the contributions of all subset sizes */
    seq_t result = 0;
    for (size_t k = 0; k <= hcnt; ++k) {
        seq_t ways = dp[(size_t)n * cols + k];
        if (ways) result += ways * fac[k];
    }

    free(dp);
    free(fac);
    return result;
}
