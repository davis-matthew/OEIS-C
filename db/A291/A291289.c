/*
 * A291289  The Padovan sequence A000931 doubled.
 * https://oeis.org/A291289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3), a(0) = 2, a(1) = 0, a(2) = 0
 * Range:     u8 n <= 23, u16 n <= 43, u32 n <= 82, u64 n <= 161,
 *            u128 n <= 319, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    if (n == 1 || n == 2)
        return (seq_t)0;

    seq_t t[3];
    t[0] = (seq_t)2;  /* a(0) */
    t[1] = (seq_t)0;  /* a(1) */
    t[2] = (seq_t)0;  /* a(2) */

    /* Compute a(n) iteratively using the recurrence */
    for (idx_t i = 3; i <= n; ++i) {
        idx_t idx = i % 3;
        seq_t val = t[(i - 2) % 3] + t[(i - 3) % 3];
        t[idx] = val;
    }

    return t[n % 3];
}
