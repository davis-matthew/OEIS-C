/*
 * A242544  Number of length n+3 0..3 arrays with no four elements in a row
 *          with pattern aabb (possibly a=b) and new values 0..3 introduced in
 *          0..3 order.
 * https://oeis.org/A242544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence:
 * a(n) = 4*a(n-1) - 3*a(n-2) + 10*a(n-3) - 2*a(n-4)
 *        - 12*a(n-5) - 9*a(n-6) - 18*a(n-7) - 9*a(n-8)
 *
 * Initial values (a(1) .. a(8)):
 * 13, 42, 150, 554, 2072, 7808, 29536, 111878
 */
seq_t a(idx_t n)
{
    /* Base values for n = 1..8 */
    static const seq_t base[8] = {
        13, 42, 150, 554, 2072, 7808, 29536, 111878
    };

    if (n <= 8)
        return base[n - 1];

    /* last[0] = a(cur-8), ..., last[7] = a(cur-1) */
    seq_t last[8];
    for (idx_t i = 0; i < 8; ++i)
        last[i] = base[i];

    for (idx_t cur = 9; cur <= n; ++cur) {
        /* Compute a(cur) using the recurrence.
         * Positive part is evaluated before the negative part
         * to avoid unsigned underflow. */
        seq_t next = (4 * last[7]) + (10 * last[5]) -
                     ((3 * last[6]) + (2 * last[4]) +
                      (12 * last[3]) + (9 * last[2]) +
                      (18 * last[1]) + (9 * last[0]));

        /* Shift the window by one position to the left.
           last[0] becomes a(cur-7), ..., last[7] becomes a(cur). */
        for (idx_t i = 0; i < 7; ++i)
            last[i] = last[i + 1];
        last[7] = next;
    }

    return last[7];
}
