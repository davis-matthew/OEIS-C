/*
 * A246059  a(n) = (17*10^n - 8)/9.
 * https://oeis.org/A246059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (17*10^n - 8)/9 Recurrence: a(0) = 1, a(n) = 10*a(n-1) +
 *            8 for n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t prev = (seq_t)1;   /* a(0) */
    seq_t cur;

    for (idx_t i = 1; i <= n; ++i) {
        cur = (seq_t)10 * prev + (seq_t)8;   /* a(i) = 10*a(i-1)+8 */

        if (i == n) {
            return cur;
        }
        prev = cur;
    }

    /* Should never reach here */
    return (seq_t)1;
}
