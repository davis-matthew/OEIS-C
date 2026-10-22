/*
 * A307465  Number of Catalan words of length n avoiding the pattern 110.
 * https://oeis.org/A307465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + a(n-4) Initial values: a(0)=1,
 *            a(1)=1, a(2)=2, a(3)=5
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle initial indices directly */
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 5;

    /* Keep the last four terms in a ring buffer */
    seq_t buf[4];
    buf[0] = 1; /* a(0) */
    buf[1] = 1; /* a(1) */
    buf[2] = 2; /* a(2) */
    buf[3] = 5; /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t new_val = 4 * buf[3] - 4 * buf[2] + buf[0];
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = new_val;
    }
    return buf[3];
}
