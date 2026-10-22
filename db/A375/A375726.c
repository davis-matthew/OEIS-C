/*
 * A375726  a(n) = a(n-1) + 3*a(n-2) + a(n-3) with a(0) = 1, a(1) = 3, a(2) =
 *          6.
 * https://oeis.org/A375726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) + a(n-3) with a(0) = 1, a(1) = 3, a(2)
 *            = 6
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial first three terms directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;
    if (n == 2)
        return (seq_t)6;

    /* a(i-3), a(i-2), a(i-1) */
    seq_t a_nm3 = (seq_t)1; /* a(0) */
    seq_t a_nm2 = (seq_t)3; /* a(1) */
    seq_t a_nm1 = (seq_t)6; /* a(2) */
    seq_t a_cur = a_nm1;

    /* Iteratively compute a(i) for i = 3 .. n */
    for (idx_t i = 3; i <= n; ++i)
    {
        a_cur = a_nm1 + (seq_t)3 * a_nm2 + a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }

    return a_cur;
}
