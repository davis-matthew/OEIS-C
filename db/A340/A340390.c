/*
 * A340390  Number of partitions of n into 4 parts such that the largest part
 *          is 3 times the smallest part.
 * https://oeis.org/A340390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 4 positive parts such that
 *            the largest part is 3 times the smallest part
 * Range:     u8 n <= 239, u16 n <= 3957, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = 0;
    for (idx_t k = 1; k <= n / 4; ++k) {
        /* smallest part = k, largest part = 3*k, remaining sum for the two middle parts */
        idx_t t = n - 4 * k;
        /* lower bound for the first middle part: max(k, t - 3*k) */
        idx_t lower = (t > 3 * k) ? (t - 3 * k) : 0;
        if (lower < k) {
            lower = k;
        }
        /* upper bound for the first middle part: min(3*k, t/2) */
        idx_t up1 = t / 2;      /* ensures m1 <= m2 */
        idx_t up2 = 3 * k;      /* ensures m2 <= 3*k */
        idx_t upper = (up1 < up2) ? up1 : up2;
        if (upper >= lower) {
            ans += (seq_t)(upper - lower + 1);
        }
    }
    return ans;
}
