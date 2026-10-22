/*
 * A348596  a(n) = A068527(2*n+1).
 * https://oeis.org/A348596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (ceiling(sqrt(2*n + 1)))^2 - (2*n + 1)
 * Range:     u8 n <= 15, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute k = 2*n + 1   (use seq_t to avoid overflow) */
    seq_t k = (seq_t)n * ((seq_t)n * 0 + 2) + 1; /* (seq_t)n * 2 + 1 */

    /* binary search for floor sqrt of k */
    seq_t low = 0;
    seq_t high = k;
    seq_t mid, sq;

    while (low <= high) {
        mid = (low + high) / 2;
        sq = mid * mid;
        if (sq == k) {          /* exact square */
            low = mid;
            break;
        }
        if (sq < k) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* low is the smallest integer greater than sqrt(k) possibly equal if k is square */
    seq_t ceil_sqrt;
    seq_t floor_sq = low * low;
    if (floor_sq == k) {
        ceil_sqrt = low;
    } else {
        ceil_sqrt = low;
    }

    /* compute a(n) */
    seq_t result = (ceil_sqrt * ceil_sqrt) - k;
    return result;
}
