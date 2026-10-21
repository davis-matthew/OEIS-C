/*
 * A282125  Number of n X 2 0..2 arrays with no element unequal to more than
 *          four of its king-move neighbors, with the exception of exactly one
 *          element, and with new values introduced in order 0 sequentially
 *          upwards.
 * https://oeis.org/A282125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 57*a(n-2) + 88*a(n-3) - 256*a(n-4) +
 *            240*a(n-5) - 320*a(n-6) + 256*a(n-7) - 64*a(n-8), with a(1)=0,
 *            a(2)=0, a(3)=30, a(4)=412, a(5)=4018, a(6)=35472, a(7)=296062,
 *            a(8)=2378276
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial terms for n = 1..8 */
    static const seq_t init[8] = {0, 0, 30, 412, 4018, 35472, 296062, 2378276};

    if (n <= 8)
        return init[n - 1];

    /* Buffer holding the last 8 values of the sequence */
    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    /* Compute successive terms up to n using the recurrence.
       Positive contributions are summed first to avoid underflow. */
    for (idx_t i = 9; i <= n; ++i)
    {
        seq_t sum_pos = 14 * buf[7] + 88 * buf[5] + 240 * buf[3] + 256 * buf[1];
        seq_t sum_neg = 57 * buf[6] + 256 * buf[4] + 320 * buf[2] + 64 * buf[0];
        seq_t next = sum_pos - sum_neg;

        /* Shift buffer left by one position and insert new term */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = buf[6];
        buf[6] = buf[7];
        buf[7] = next;
    }

    return buf[7];
}
