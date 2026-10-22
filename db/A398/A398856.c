/*
 * A398856  Lower (2/3, 1)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5); a(0) = 0,
 *            a(1) = 1, a(2) = 7, a(3) = 16, a(4) = 28
 * Range:     u8 n <= 11, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)16;
    if (n == 4) return (seq_t)28;

    /* last five values a(i-1), a(i-2), a(i-3), a(i-4), a(i-5) */
    seq_t t0 = (seq_t)28, t1 = (seq_t)16, t2 = (seq_t)7, t3 = (seq_t)1, t4 = (seq_t)0;

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = 2*t0 - t1 + t2 - 2*t3 + t4;
        t4 = t3;
        t3 = t2;
        t2 = t1;
        t1 = t0;
        t0 = next;
    }
    return t0;
}
