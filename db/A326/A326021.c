/*
 * A326021  Number of complete subsets of {1..n} with maximum n.
 * https://oeis.org/A326021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of complete subsets of {1..n} with maximum n
 *            Computation: Use dynamic programming on the partial sums. A
 *            subset is complete iff 1 is in it and every successive element a
 *            satisfies a ≤ 1 + sum_of_smaller_elements. While iterating
 *            through the numbers 2 … n-1 we keep two kinds of state...
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small n are trivial */
    if (n <= 2) return 1;          /* {1} and {1,2} */

    /* dp holds counts for sums 0 .. n-2  (size n-1) */
    size_t sz = (size_t)(n - 1);
    seq_t *dp = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *newdp = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!dp || !newdp) return 0;  /* allocation failure */

    dp[1] = 1;                     /* start with subset {1}   */
    seq_t count_above = 0;          /* subsets with sum ≥ n-1  */

    for (idx_t i = 2; i <= n - 1; ++i) {
        /* reset newdp to zero */
        for (size_t k = 0; k < sz; ++k) newdp[k] = 0;

        /* existing “above” subsets double in count (include/exclude i) */
        count_above *= 2;

        for (idx_t s = 1; s < n - 1; ++s) {
            seq_t val = dp[s];
            if (!val) continue;

            /* exclude i */
            newdp[s] += val;

            /* include i if allowed */
            if (i <= s + 1) {
                idx_t s2 = s + i;
                if (s2 < n - 1) {
                    newdp[s2] += val;
                } else {
                    count_above += val;   /* sum now ≥ n-1 */
                }
            }
        }

        /* swap dp and newdp for next iteration */
        seq_t *tmp = dp; dp = newdp; newdp = tmp;
    }

    seq_t ans = count_above;
    free(dp);
    free(newdp);
    return ans;
}
