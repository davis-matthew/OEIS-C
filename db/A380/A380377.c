/*
 * A380377  Minimum number of total votes needed for one party to win if there
 *          are n voters divided into balanced districts, i.e., the numbers of
 *          voters in two districts may differ by at most 1.
 * https://oeis.org/A380377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 973, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = min_{1 <= d <= n} { minimal total votes needed to win a strict majority of districts 
 *            when the n voters are split into d balanced districts (sizes differ by at most 1).
 *          }
 * The minimal votes for a given d are derived from the W = floor(d/2)+1 districts that must be won 
 * and from the thresholds t_small = floor(q/2)+1 (for a district of size q) and t_large = floor((q+1)/2)+1 (for a district of size q+1),
 * where q = floor(n/d) and r = n % d.  
 * If there are at least W districts of size q, the optimal choice is to win those W; otherwise we win all size‑q districts and the rest from size‑q+1 districts.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, but safeguard */

    idx_t best = n + 1;  /* larger than any possible minimal number of votes */

    for (idx_t d = 1; d <= n; ++d) {
        idx_t q = n / d;            /* floor(n/d)  */
        idx_t r = n % d;            /* number of districts of size q+1  */
        idx_t small_cnt = d - r;    /* number of districts of size q   */
        idx_t t_small = (q / 2) + 1;               /* threshold for size q          */
        idx_t t_large = ((q + 1) / 2) + 1;         /* threshold for size q+1        */
        idx_t win_needed = (d / 2) + 1;            /* districts that must be won    */

        idx_t needed_votes;
        if (small_cnt >= win_needed) {
            needed_votes = win_needed * t_small;
        } else {
            needed_votes = small_cnt * t_small
                         + (win_needed - small_cnt) * t_large;
        }

        if (needed_votes < best) best = needed_votes;
    }

    return (seq_t)best;
}
