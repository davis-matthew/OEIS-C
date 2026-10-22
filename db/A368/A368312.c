/*
 * A368312  Irregular triangle read by rows where row n lists the factor
 *          differences of n.
 * https://oeis.org/A368312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is the n-th element of the irregular triangle of
 *            factor-difference values, read row by row. Each row n contains
 *            the absolute differences p - q for each factor pair n = p * q
 *            with p ≥ √n, listed in ascending order
 * Range:     u8 n <= 741, u16 n <= 9766, u32 n <= 9766, u64 n <= 9766,
 *            u128 n <= 9766, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t floor_sqrt_u64(idx_t n)
{
    /* binary search for floor(sqrt(n)) */
    idx_t low = 1;
    idx_t high = n;
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid <= n / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t pos)
{
    idx_t count = 0;               /* number of terms produced so far */

    for (idx_t n = 1; ; ++n) {
        idx_t sq = floor_sqrt_u64(n);

        for (idx_t d = sq; d >= 1; --d) {
            if (n % d == 0) {
                idx_t p = n / d;          /* p ≥ d because d ≤ √n */
                seq_t diff = (seq_t)p - (seq_t)d;
                ++count;
                if (count == pos) {
                    return diff;
                }
            }
        }
    }
}
