/*
 * A291023  p-INVERT of (1,1,1,1,1,...), where p(S) = 1 - 3 S^2 + 2 S^3.
 * https://oeis.org/A291023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * a(n-1) - 4 * a(n-3) for n >= 4, with seed values
 *            a(0)=0, a(1)=3, a(2)=4, a(3)=12
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)12;

    /* Use a rolling window of the last four computed terms. */
    seq_t buf[4] = { (seq_t)0, (seq_t)3, (seq_t)4, (seq_t)12 };

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = 3 * buf[3] - 4 * buf[1];
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = next;
    }
    return buf[3];
}
