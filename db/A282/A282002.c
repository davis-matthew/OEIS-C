/*
 * A282002  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 413", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=10, a(2)=11, a(3)=1110, a(4)=11, a(n) = 101*a(n-2)
 *            - 100*a(n-4) for n > 4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)1110;
    if (n == 4) return (seq_t)11;

    seq_t buf[5];
    buf[0] = (seq_t)1;       /* a0 */
    buf[1] = (seq_t)10;      /* a1 */
    buf[2] = (seq_t)11;      /* a2 */
    buf[3] = (seq_t)1110;    /* a3 */
    buf[4] = (seq_t)11;      /* a4 */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)101 * buf[3] - (seq_t)100 * buf[1];
        /* shift window for next iteration */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = next;
    }
    return buf[4];
}
