/*
 * A347026  Irregular triangle read by rows in which row n lists the first n
 *          odd numbers, followed by the first n odd numbers in decreasing
 *          order.
 * https://oeis.org/A347026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 2k-1 if 1 <= k <= r 4r-2k+1 if r+1 <= k <= 2r where r =
 *            ceil( (-1 + sqrt(1 + 4n)) / 2 ) is the row number, and k = n -
 *            r(r-1) is the position of n within that row
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* offset is 1; undefined for n=0 */
    seq_t nd = (seq_t)n;
    /* binary search for smallest r with r(r+1) >= n */
    idx_t low = 0, high = n;         /* high <= n is safe */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((seq_t)mid * (mid + 1) >= nd) {
            high = mid;
        } else {
            low = mid;
        }
    }
    idx_t r = high;
    seq_t prev = (seq_t)(r - 1) * r;    /* terms before this row */
    seq_t k = nd - prev;                /* 1‑based position in row */
    seq_t val;
    if (k <= (seq_t)r) {
        val = 2 * k - 1;
    } else {
        val = (seq_t)4 * r - 2 * k + 1;
    }
    return val;
}
