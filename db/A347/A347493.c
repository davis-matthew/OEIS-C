/*
 * A347493  a(0) = 1, a(1) = 0, a(2) = a(3) = 1; thereafter, a(n) = a(n-1) +
 *          a(n-2) + a(n-4).
 * https://oeis.org/A347493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-4); a(0) = 1, a(1) = 0, a(2) = 1,
 *            a(3) = 1
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 41, u64 n <= 81, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)1;

    /* buffer holds a(i-4), a(i-3), a(i-2), a(i-1) for current i */
    seq_t buf[4] = { (seq_t)1, (seq_t)0, (seq_t)1, (seq_t)1 };
    seq_t result = buf[3]; /* initialize with a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        result = buf[3] + buf[2] + buf[0];  /* a(i-1) + a(i-2) + a(i-4) */
        /* shift buffer: drop a(i-4), shift left */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = result;
    }
    return result;
}
