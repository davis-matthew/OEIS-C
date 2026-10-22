/*
 * A329244  Sum of every third term of the Padovan sequence A000931.
 * https://oeis.org/A329244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 3*a(n-3) - a(n-4) a(0) = 1, a(1) =
 *            2, a(2) = 3, a(3) = 5
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;

    seq_t p0 = (seq_t)1;   /* a(n-4) */
    seq_t p1 = (seq_t)2;   /* a(n-3) */
    seq_t p2 = (seq_t)3;   /* a(n-2) */
    seq_t p3 = (seq_t)5;   /* a(n-1) */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i) {
        cur = 4 * p3 - 5 * p2 + 3 * p1 - p0;
        p0 = p1;
        p1 = p2;
        p2 = p3;
        p3 = cur;
    }

    return p3;
}
