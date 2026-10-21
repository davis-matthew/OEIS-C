/*
 * A273386  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 659", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4), with initial
 *            values a(0)=1, a(1)=6, a(2)=23, a(3)=72, a(4)=153, a(5)=274
 *            (Recurrence holds for n > 5) This is the partial-sum sequence
 *            A273386
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the known initial terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)23;
    if (n == 3) return (seq_t)72;
    if (n == 4) return (seq_t)153;
    if (n == 5) return (seq_t)274;

    /* Sliding window for the recurrence:
       buf[0] = a(i-1), buf[1] = a(i-2), buf[2] = a(i-3), buf[3] = a(i-4)  */
    seq_t buf[4] = {274, 153,   72,   23};   /* a(5), a(4), a(3), a(2)   */

    for (idx_t i = 6; i <= n; ++i) {
        /* Compute a(i) = 4*a(i-1) - 6*a(i-2) + 4*a(i-3) - a(i-4) */
        seq_t next = (buf[0] * (seq_t)4)
                   - (buf[1] * (seq_t)6)
                   + (buf[2] * (seq_t)4)
                   - buf[3];

        /* Shift the window: drop a(i-4) and slide values */
        buf[3] = buf[2];
        buf[2] = buf[1];
        buf[1] = buf[0];
        buf[0] = next;
    }

    return buf[0];   /* After loop, buf[0] holds a(n) */
}
