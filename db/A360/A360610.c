/*
 * A360610  Triangle read by rows: T(n,k) is the number of squares of side
 *          length k that can be placed inside a square of side length n
 *          without overlap, 1 <= k <= n.
 * https://oeis.org/A360610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor(row/col)^2 where * row = the smallest integer r
 *            with r*(r+1)/2 ≥ n * col = n − r*(r−1)/2 * (row, col) correspond
 *            to the (r‑th row, k‑th column) of the triangle * T(r,k) =
 *            floor(r/k)^2
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the row r for index n (1‑based) in the triangle read by rows. */
static idx_t
find_row(idx_t n)
{
    idx_t low = 1, high = 1;
    /* Increase high until it covers n */
    while (((__int128)high * (high + 1)) >> 1 < n)
        high <<= 1;

    /* Binary search for the smallest r with r*(r+1)/2 ≥ n */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 val = ((__int128)mid * (mid + 1)) >> 1;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t
a(idx_t n)
{
    /* row r */
    idx_t r = find_row(n);
    /* previous cumulative count: (r-1)*r/2 */
    idx_t prev = ((__int128)(r - 1) * r) >> 1;
    /* column k */
    idx_t k = n - prev;
    /* floor(r/k) */
    idx_t q = r / k;
    /* result = q^2 in seq_t */
    return (seq_t)q * (seq_t)q;
}
