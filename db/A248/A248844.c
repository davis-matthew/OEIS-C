/*
 * A248844  Triangle read by rows: T(m,n) = number of ways of distributing n
 *          distinguishable balls into m distinguishable bins of size 2 where
 *          empty bins are permitted (m >= 1, 1 <= n <= 2m).
 * https://oeis.org/A248844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 28, u32 n <= 54, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A248844: Triangle T(m,n) = number of ways of distributing n distinguishable balls
 * into m distinguishable bins of size 2 (empty bins allowed).
 * The sequence a(n) is read row‐by‐row: a(1)=T(1,1), a(2)=T(1,2), a(3)=T(2,1), …
 *
 * The computation uses the closed form
 *   T(m,n) = Σ_{k=max(0,n-m)}^{⌊n/2⌋}
 *            C(m,k) · (n!/(n-2k)!)/2^k · (m-k)!/(m-n+k)!
 * which follows from choosing k bins with two balls, selecting the ball pairs,
 * and assigning the remaining n-2k balls to distinct bins.
 */
#include <math.h>

seq_t a(idx_t idx)
{
    /* determine row m and column j (i.e. n) from flattened index */
    idx_t D = 1 + 4 * idx;
    idx_t t = (idx_t)sqrt((long double)D);
    while (t*t > D) --t;
    while ((t+1)*(t+1) <= D) ++t;
    idx_t m = (t - 1) / 2;
    if (m * (m + 1) < idx) ++m;
    idx_t prev_sum = (m - 1) * m;           /* terms before row m */
    idx_t n = idx - prev_sum;               /* column within row (1-based) */

    /* compute T(m,n) by summation over k */
    seq_t total = 0;
    idx_t k_min = n > m ? n - m : 0;        /* lower bound on k */
    idx_t k_max = n / 2;                    /* upper bound on k */

    for (idx_t k = k_min; k <= k_max; ++k) {
        /* comb = C(m,k) */
        seq_t comb = 1;
        for (idx_t i = 1; i <= k; ++i)
            comb = comb * (seq_t)(m - k + i) / (seq_t)i;

        /* part1 = n!/(n-2k)! divided by 2^k */
        seq_t part1 = 1;
        idx_t start1 = n - 2*k + 1;
        for (idx_t i = start1; i <= n; ++i)
            part1 *= (seq_t)i;
        for (idx_t i = 0; i < k; ++i)
            part1 /= (seq_t)2;

        /* part2 = (m-k)!/(m-n+k)! */
        seq_t part2 = 1;
        idx_t start2 = m - n + k + 1;
        idx_t end2   = m - k;
        if (start2 <= end2) {
            for (idx_t i = start2; i <= end2; ++i)
                part2 *= (seq_t)i;
        }

        total += comb * part1 * part2;
    }

    return total;
}
