/*
 * A244936  Number of length n 0..4 arrays with each partial sum starting from
 *          the beginning no more than sqrt(3) standard deviations from its
 *          mean.
 * https://oeis.org/A244936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A244936
   Formula: a(n) = number of length‑n arrays with elements in {0,1,2,3,4}
   such that for every prefix of length k (1 ≤ k ≤ n) the partial sum S_k
   satisfies (S_k − 2k)² ≤ 6k.  The implementation DP‑s over the deviation
   d_k = S_k−2k.  The state space is bounded by |d_k| ≤ ⌊√(6k)⌋.
*/

#include <stdlib.h>
#include <stdint.h>

static inline idx_t floor_sqrt(idx_t n)
{
    /* integer floor of sqrt(n) using Newton's method */
    if (n < 2)
        return n;
    idx_t x = n;
    idx_t y = (x + n / x) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* offset 1 */

    /* maximum possible deviation for the final step */
    idx_t max_d = floor_sqrt(6 * n) + 2;   /* safety margin */
    size_t sz   = 2 * (size_t)max_d + 1;
    size_t offset = (size_t)max_d;

    seq_t *dp_prev = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp_prev || !dp_next) {
        free(dp_prev);
        free(dp_next);
        return 0;
    }

    dp_prev[offset] = 1;   /* initial state d_0 = 0 */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t step_limit = 6 * k;                   /* 6k fits in idx_t */
        /* reset next buffer */
        for (size_t i = 0; i < sz; ++i)
            dp_next[i] = 0;

        for (size_t i = 0; i < sz; ++i) {
            seq_t ways = dp_prev[i];
            if (!ways)
                continue;

            int d = (int)i - (int)offset;          /* current deviation */

            /* try each possible element: -2..2 increment to d */
            for (int inc = -2; inc <= 2; ++inc) {
                int nd = d + inc;                  /* new deviation */
                if ((seq_t)((nd) * (nd)) > (seq_t)step_limit)
                    continue;                     /* outside bounds */

                size_t idx = (size_t)(nd + (int)offset);
                dp_next[idx] += ways;
            }
        }
        /* swap for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_next;
        dp_next = tmp;
    }

    /* sum all ways after final step */
    seq_t total = 0;
    for (size_t i = 0; i < sz; ++i)
        total += dp_prev[i];

    free(dp_prev);
    free(dp_next);
    return total;
}
