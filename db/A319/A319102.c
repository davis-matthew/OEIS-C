/*
 * A319102  Triangle read by rows: The k-th column has alternating blocks of k
 *          1's followed by k 0's (see example).
 * https://oeis.org/A319102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (floor(r / k) mod 2), where r is the smallest integer
 *            such that r(r+1)/2 >= n (row index) and k = n - r(r-1)/2 (column
 *            index within that row)
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* Offset starts at 1 */

    /* Find r: smallest integer with r*(r+1)/2 >= n */
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
    idx_t r = high;

    /* Compute column index k */
    __int128 prev_sum = (__int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev_sum;

    /* Result is floor(r/k) mod 2 */
    return (seq_t)((r / k) & 1);
}
