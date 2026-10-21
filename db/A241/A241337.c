/*
 * A241337  Number of partitions p of n not including ceiling(mean(p)) as a
 *          part.
 * https://oeis.org/A241337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = Σ_{k=1}^{n} ( p_k(n) - p_{k-1}( n - ⎡n/k⎤ ) ) * where
 *            p_k(m) is the number of partitions of m into exactly k parts, *
 *            p_0(0)=1 and all other p_0(m>0)=0. * For n=0 we return 1 as
 *            prescribed by the OEIS offset
 * Range:     u8 n <= 18, u16 n <= 47, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* arrays holding p_{k-1} and p_k for all 0≤m≤n */
    seq_t *prev = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!prev || !cur) {
        /* allocation failure – return 0 as a safe default   */
        free(prev); free(cur);
        return (seq_t)0;
    }

    /* p_0(0)=1 */
    prev[0] = (seq_t)1;

    seq_t ans = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        /* initialize cur[j] = 0 for j<k   */
        for (idx_t j = 0; j < k; ++j)
            cur[j] = (seq_t)0;

        /* compute cur[j] = cur[j-k] + prev[j-1]   */
        for (idx_t j = k; j <= n; ++j)
            cur[j] = cur[j - k] + prev[j - 1];

        /* ceil(n/k) */
        idx_t r = (n + k - 1) / k;

        /* subtract partitions that contain the part r   */
        seq_t sub = (seq_t)0;
        if (n >= r) {
            idx_t left = n - r;
            /* prev[left] is p_{k-1}(left)   */
            sub = prev[left];
        }

        ans += cur[n] - sub;

        /* prepare for next iteration   */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }

    free(prev);
    free(cur);
    return ans;
}
