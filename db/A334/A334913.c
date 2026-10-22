/*
 * A334913  a(n) is the sum of digits of n in signed binary nonadjacent form.
 * https://oeis.org/A334913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = A000120(3*n-1) - A000120(n-1) (binary popcount
 *            difference)
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t popcount_idx(idx_t x) {
    unsigned long long cnt = 0;
    while (x) {
        cnt++;
        x &= x - 1;   /* drop lowest set bit */
    }
    return (seq_t)cnt;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    idx_t x = 3 * n - 1;
    idx_t y = n - 1;
    seq_t cx = popcount_idx(x);
    seq_t cy = popcount_idx(y);
    return cx - cy;
}
