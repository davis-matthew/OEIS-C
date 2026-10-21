/*
 * A263948  Positive integers n such that (n+61)^3 - n^3 is a square.
 * https://oeis.org/A263948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 975*a(n-1) - 975*a(n-2) + a(n-3) Initial values:
 *            a(1)=244, a(2)=267607, a(3)=260678620
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 1) return (seq_t)244;
    if (n == 2) return (seq_t)267607;
    if (n == 3) return (seq_t)260678620;

    /* Use a sliding window of the last three terms. */
    seq_t a1 = (seq_t)244;        /* a(n-3) */
    seq_t a2 = (seq_t)267607;     /* a(n-2) */
    seq_t a3 = (seq_t)260678620;  /* a(n-1) */

    seq_t next;
    idx_t i;
    for (i = 4; i <= n; ++i) {
        next = (seq_t)975 * a3;
        next -= (seq_t)975 * a2;
        next += a1;

        /* shift the window */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
