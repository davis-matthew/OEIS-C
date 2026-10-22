/*
 * A341735  a(n) = A007678(2*n+1).
 * https://oeis.org/A341735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n - 1)*(2*n^2 - n + 5)/6
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* work in idx_t to avoid overflow in the intermediate calculations   */
    idx_t n_idx = n;
    idx_t y_idx = 2 * n_idx - 1;
    idx_t z_idx = 2 * n_idx * n_idx - n_idx + 5;

    seq_t x = (seq_t)n_idx;
    seq_t y = (seq_t)y_idx;
    seq_t z = (seq_t)z_idx;

    /* cancel the factor 2 */
    if ((x & 1) == 0) {
        x >>= 1;
    } else if ((y & 1) == 0) {
        y >>= 1;
    } else {
        z >>= 1;
    }

    /* cancel the factor 3 */
    if (x % 3 == 0) {
        x /= 3;
    } else if (y % 3 == 0) {
        y /= 3;
    } else {
        z /= 3;
    }

    return x * y * z;
}
