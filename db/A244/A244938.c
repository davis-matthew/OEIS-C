/*
 * A244938  Number of length n 0..6 arrays with each partial sum starting from
 *          the beginning no more than sqrt(3) standard deviations from its
 *          mean.
 * https://oeis.org/A244938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n): count of length‑n arrays of integers 0..6 such that for
 * each k (1 ≤ k ≤ n) the partial sum S_k satisfies
 * |S_k – 3k| ≤ 2·sqrt(3k).
 *
 * Computation is by dynamic programming over partial sums.
 * The algorithm runs in O(n·√n) time and O(n) memory.
 */
#include <stdlib.h>

/* integer square root: floor(sqrt(n)) */
static idx_t isqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset starts at 1 */

    /* maximum possible sum: 6 per position */
    idx_t max_sum = 6 * n;
    seq_t *dp_cur  = (seq_t *)calloc(max_sum + 1, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(max_sum + 1, sizeof(seq_t));
    if (!dp_cur || !dp_next) {
        /* allocation failure: return 0 gracefully */
        free(dp_cur);
        free(dp_next);
        return 0;
    }

    /* initial state: sum 0 is reachable in 1 way */
    dp_cur[0] = 1;
    idx_t prevL = 0, prevR = 0;   /* bounds for k‑1 */

    for (idx_t k = 1; k <= n; ++k) {
        /* bounds for partial sum after k steps:
         * L_k = 3k – floor(sqrt(12k))
         * R_k = 3k + floor(sqrt(12k))
         */
        idx_t t12   = 12 * k;
        idx_t sq12  = isqrt(t12);
        idx_t Lk    = 3 * k;
        if (Lk >= sq12)
            Lk -= sq12;
        else
            Lk = 0;
        idx_t Rk = 3 * k + sq12;

        /* zero next array over its active range */
        for (idx_t s = Lk; s <= Rk; ++s)
            dp_next[s] = 0;

        /* transition from previous states */
        for (idx_t s_old = prevL; s_old <= prevR; ++s_old) {
            seq_t val = dp_cur[s_old];
            if (val == 0)
                continue;
            for (idx_t step = 0; step <= 6; ++step) {
                idx_t s_new = s_old + step;
                if (s_new < Lk || s_new > Rk)
                    continue;
                dp_next[s_new] += val;
            }
        }

        /* swap buffers */
        seq_t *tmp   = dp_cur;
        dp_cur       = dp_next;
        dp_next      = tmp;
        prevL = Lk;
        prevR = Rk;
    }

    /* sum all reachable states after n steps */
    seq_t total = 0;
    for (idx_t s = prevL; s <= prevR; ++s)
        total += dp_cur[s];

    free(dp_cur);
    free(dp_next);
    return total;
}
