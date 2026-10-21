/*
 * A242639  Array read by antidiagonals upwards: A(s,n) (s>=1, n >= 1) =
 *          Sum_{d|n, d <= s} d^2 + s*Sum_{d|n, d>s} d.
 * https://oeis.org/A242639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read from the array A(s, t) defined by A(s, t) = Σ_{d|t,
 *            d≤s} d² + s·Σ_{d|t, d>s} d. The index n is mapped to an
 *            antidiagonal by increasing the sum k = s + t and listing
 *            elements with s descending
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find minimal m such that m*(m+1)/2 ≥ n.  Here m = k - 1. */
    idx_t low = 0, high = 1;
    while ((__int128)high * (high + 1) / 2 < n)
        high <<= 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid;
    }
    idx_t m = high;           /* m >= 1 for n ≥ 1 */
    idx_t k = m + 1;          /* diagonal index (s + t) */
    idx_t prev_total = (idx_t)(((__int128)(m - 1) * m) / 2); /* sum up to diag k-1 */
    idx_t pos = n - prev_total;                              /* 1‑based position in diagonal */
    idx_t s = k - pos;           /* row index */
    idx_t j = pos;               /* column index */

    seq_t sum_sq = 0;
    seq_t sum_gt = 0;
    for (idx_t d = 1; d * d <= j; ++d) {
        if (j % d == 0) {
            idx_t d2 = j / d;
            if (d <= s)
                sum_sq += (seq_t)d * d;
            else
                sum_gt += d;

            if (d2 != d) {
                if (d2 <= s)
                    sum_sq += (seq_t)d2 * d2;
                else
                    sum_gt += d2;
            }
        }
    }
    return sum_sq + (seq_t)s * sum_gt;
}
