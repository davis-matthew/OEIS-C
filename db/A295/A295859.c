/*
 * A295859  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = -2,
 *          a(1) = 0, a(2) = 1, a(3) = 1.
 * https://oeis.org/A295859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4), a(0) = -2, a(1)
 *            = 0, a(2) = 1, a(3) = 1
 * Range:     i8 n <= 8, i16 n <= 19, i32 n <= 42, i64 n <= 88, i128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial cases directly. */
    if (n == 0) return (seq_t)-2;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)1;

    /* Sliding window of the last four values. */
    seq_t a0 = (seq_t)-2; /* a(n-4) */
    seq_t a1 = (seq_t)0;  /* a(n-3) */
    seq_t a2 = (seq_t)1;  /* a(n-2) */
    seq_t a3 = (seq_t)1;  /* a(n-1) */

    /* Compute successive terms up to n. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = a3 + (seq_t)3 * a2 - (seq_t)2 * a1 - (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
