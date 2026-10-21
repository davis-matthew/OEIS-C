/*
 * A292295  Sum of values of vertices of type A at level n of the hyperbolic
 *          Pascal pyramid.
 * https://oeis.org/A292295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + 2*a(n-3) (for n >= 4, with a(0) =
 *            0, a(1) = 0, a(2) = 6, a(3) = 18)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)6;
    if (n == 3)
        return (seq_t)18;

    /* initialise a(n-3), a(n-2), a(n-1) for n = 4 start */
    seq_t a_nm3 = (seq_t)0;   /* a(1) */
    seq_t a_nm2 = (seq_t)6;   /* a(2) */
    seq_t a_nm1 = (seq_t)18;  /* a(3) */
    seq_t an    = a_nm1;      /* will hold a(n) */

    for (idx_t i = 4; i <= n; ++i) {
        an     = (seq_t)5 * a_nm1 - (seq_t)6 * a_nm2 + (seq_t)2 * a_nm3;
        a_nm3  = a_nm2;
        a_nm2  = a_nm1;
        a_nm1  = an;
    }
    return an;
}
