/*
 * A291010  p-INVERT of (1,1,1,1,1,...), where p(S) = (1 - 2*S)*(1 - 3*S).
 * https://oeis.org/A291010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 12*a(n-2), a(0) = 5, a(1) = 24 (equivalently
 *            a(n) = 9*4^n - 4*3^n) This implements the OEIS sequence A291010
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)5;
    if (n == 1)
        return (seq_t)24;

    seq_t a0 = (seq_t)5;   /* a(0) */
    seq_t a1 = (seq_t)24;  /* a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)7 * a1 - (seq_t)12 * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
