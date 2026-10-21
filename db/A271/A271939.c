/*
 * A271939  Number of edges in the n-th order Sierpinski carpet graph.
 * https://oeis.org/A271939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 24*a(n-2), a(1)=8, a(2)=88
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 0)
        return (seq_t)0;       /* undefined offset */
    if (n == 1)
        return (seq_t)8;
    if (n == 2)
        return (seq_t)88;

    seq_t a_nm2 = (seq_t)8;   /* a(1) */
    seq_t a_nm1 = (seq_t)88;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t a_n = (seq_t)11 * a_nm1 - (seq_t)24 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
