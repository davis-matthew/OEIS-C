/*
 * A399086  Triangle read by rows: T(n,k) is the number of ordered outcomes
 *          with product k when two independent fair n-sided dice, each with
 *          faces 0,...,n-1, are rolled, for 0 <= k <= (n-1)^2.
 * https://oeis.org/A399086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the flattened triangle entry T(m,k), where *m* is the
 *            smallest integer with Σ_{i=1}^{m}((i-1)^2+1) ≥ n and *k* = n -
 *            Σ_{i=1}^{m-1}((i-1)^2+1) - 1. For each row n: T(n,0)=2*n-1; for
 *            k>0, T(n,k)=#{ d | k : floor(k/n) < d < n }. The implementation
 *            first finds the row *m* by binary search on...
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* cumulative number of entries in rows 1 .. m */
static seq_t cum_len(idx_t m)
{
    if (m == 0) return 0;
    seq_t rm1 = (seq_t)(m - 1);
    seq_t r   = (seq_t)m;
    seq_t sum_sq = rm1 * r * ((seq_t)(2 * m - 1)) / 6;   /* Σ_{j=0}^{m-1} j^2 */
    return sum_sq + r;                                   /* add the m rows themselves */
}

seq_t a(idx_t N)
{
    /* 1. Find the row m such that cum_len(m-1) < N ≤ cum_len(m) */
    idx_t low  = 1;
    idx_t high = 1;
    /* exponential phase */
    while (cum_len(high) < (seq_t)N) high <<= 1;
    /* binary search */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cum_len(mid) >= (seq_t)N)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;                                   /* row number (1‑based) */

    /* 2. Position within that row (0‑based) */
    seq_t before = cum_len(m - 1);                    /* sum of previous rows */
    seq_t pos_seq = (seq_t)N - before - 1;            /* φ = n - L(m-1) - 1 */
    idx_t k = (idx_t)pos_seq;                          /* column index */

    /* 3. Evaluate T(m,k) */
    if (k == 0) return (seq_t)(2 * m - 1);

    seq_t count = 0;
    idx_t floor_k_over_m = k / m;                     /* ⌊k/m⌋ */
    for (idx_t d = 1; (idx_t)(d * d) <= k; ++d) {
        if (k % d == 0) {
            /* divisor d */
            if (floor_k_over_m < d && d < m) ++count;
            /* paired divisor k/d */
            idx_t d2 = k / d;
            if (d2 != d && floor_k_over_m < d2 && d2 < m) ++count;
        }
    }
    return count;
}
