/*
 * A316709  Bisection of the odd-indexed Pell numbers A001653: part 2.
 * https://oeis.org/A316709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Pell(4*n+3) = 34*a(n-1) - a(n-2); Initial values: a(0) =
 *            5, a(1) = 169
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)169;
    seq_t a0 = (seq_t)5;
    seq_t a1 = (seq_t)169;
    seq_t an;
    for (idx_t i = 2; i <= n; ++i)
    {
        an = (seq_t)34 * a1 - a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
