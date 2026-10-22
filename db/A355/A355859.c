/*
 * A355859  Triangle read by rows: T(n,k) = (n + k)/2 if (n + k) is congruent
 *          to 0 (mod 2), otherwise T(n,k) = 0; n >= 1, k >= 1.
 * https://oeis.org/A355859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r + k)/2 if (r + k) even else 0, where r = smallest
 *            integer such that r*(r+1)/2 >= n and k = n - r*(r-1)/2
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Assume n >= 1. */
    idx_t lo = 1, hi = 1;
    /* Find an upper bound hi so that HI*(HI+1)/2 >= n. */
    while (((unsigned __int128)hi * (hi + 1)) / 2 < n &&
           hi < UINT64_MAX / 2)
        hi <<= 1;
    /* Binary search for the minimal r. */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (((unsigned __int128)mid * (mid + 1)) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;
    /* Number of elements before row r. */
    idx_t prev = (idx_t)(((unsigned __int128)(r - 1) * r) / 2);
    /* Column index within row r. */
    idx_t k = n - prev;
    idx_t sum = r + k;
    if ((sum & 1) == 0)
        return (seq_t)(sum / 2);
    else
        return (seq_t)0;
}
