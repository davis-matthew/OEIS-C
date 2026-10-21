/*
 * A293067  Sum of values of vertices of type A at level n of the hyperbolic
 *          Pascal pyramid PP_(4,5).
 * https://oeis.org/A293067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=2, a(3)=6, a(n)=5*a(n-1)-6*a(n-2)+2*a(n-3)
 *            for n>=4
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return the first four terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;

    /* a(n-3), a(n-2), a(n-1) for the current n */
    seq_t a_nm3 = (seq_t)0;   /* a(1) */
    seq_t a_nm2 = (seq_t)2;   /* a(2) */
    seq_t a_nm1 = (seq_t)6;   /* a(3) */
    seq_t an    = (seq_t)0;   /* current term (n>=4) */

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)5 * a_nm1 - (seq_t)6 * a_nm2 + (seq_t)2 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }

    return an;
}
