/*
 * A276601  Values of k such that k^2 + 9 is a triangular number (A000217).
 * https://oeis.org/A276601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) - a(n-4), with base values a(1)=1, a(2)=6,
 *            a(3)=12, a(4)=37
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the base cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)12;
    if (n == 4) return (seq_t)37;

    /* t0 = a(i-4), t1 = a(i-3), t2 = a(i-2), t3 = a(i-1) */
    seq_t t0 = (seq_t)1;   /* a(1) */
    seq_t t1 = (seq_t)6;   /* a(2) */
    seq_t t2 = (seq_t)12;  /* a(3) */
    seq_t t3 = (seq_t)37;  /* a(4) */
    seq_t newval = 0;

    for (idx_t i = 5; i <= n; ++i) {
        newval = (seq_t)6 * t2 - t0;  /* recurrence */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = newval;
    }
    return newval;
}
