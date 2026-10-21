/*
 * A291034  p-INVERT of the positive integers, where p(S) = 1 - 7*S.
 * https://oeis.org/A291034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - a(n-2), a(0) = 7, a(1) = 63
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)7;
    if (n == 1) return (seq_t)63;

    seq_t a0 = 7;
    seq_t a1 = 63;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t a2 = 9 * a1 - a0;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
