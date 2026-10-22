/*
 * A391618  Chebyshev distance from n to the origin in a square spiral of
 *          positive integers and 1 at the origin.
 * https://oeis.org/A391618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((sqrt(n - 1) + 1) / 2)
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_idx(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = x;
    idx_t ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) {
            ans = 0;
            lo = 1;
            continue;
        }
        if (mid <= x / mid) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* behaviour below offset is irrelevant */
    idx_t m = n - 1;
    idx_t r = isqrt_idx(m);
    return (seq_t)((r + 1) / 2);
}
