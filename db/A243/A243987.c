/*
 * A243987  Triangle read by rows: T(n, k) is the number of divisors of n that
 *          are less than or equal to k for 1 <= k <= n.
 * https://oeis.org/A243987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of divisors of r that are <= c, where r is the row
 *            index satisfying r*(r-1)/2 < n <= r*(r+1)/2, and c = n -
 *            r*(r-1)/2
 * Range:     u8 n <= 7875, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* safety: sequence is 1‑based */

    /* Find the row r via binary search on the triangular numbers. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column index c within the row. */
    idx_t prev_sum = (r > 1) ? (idx_t)((unsigned __int128)(r - 1) * r / 2) : 0;
    idx_t c = n - prev_sum;

    /* Count divisors of r that are <= c. */
    seq_t count = 0;
    idx_t limit = (idx_t)floor(sqrt((long double)r));
    for (idx_t i = 1; i <= limit; ++i) {
        if (r % i == 0) {
            if (i <= c) ++count;
            idx_t j = r / i;
            if (j != i && j <= c) ++count;
        }
    }
    return count;
}
