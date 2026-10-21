/*
 * A245621  Sequence of distinct least nonnegative numbers such that the
 *          average of the first n terms is a cube.
 * https://oeis.org/A245621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2*n) = 7*n³ - 9*n² + 5*n - 1 Formula: a(2*n+1) = n³
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <assert.h>
seq_t a(idx_t n)
{
    assert(n >= 1);
    if (n & 1U) {          /* odd: a(2*n+1) = n³ */
        seq_t m = (seq_t)(n >> 1);
        return m * m * m;
    } else {               /* even: a(2*n) = 7n³ - 9n² + 5n - 1 */
        seq_t m = (seq_t)(n >> 1);
        seq_t t1 = 7 * m * m * m;   /* 7*n³   */
        seq_t sub = 9 * m * m;      /* 9*n²   */
        sub = sub - 5 * m + 1;      /* 9n² - 5n + 1 */
        return t1 - sub;            /* 7n³ - (9n² - 5n + 1) */
    }
}
