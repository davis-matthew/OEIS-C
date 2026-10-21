/*
 * A250777  Number of (n+1) X (2+1) 0..1 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2), a(1)=21, a(2)=46
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ul)
        return (seq_t)0;          /* undefined for offset 1, but harmless */

    if (n == 1ul)
        return (seq_t)21;         /* first term */

    if (n == 2ul)
        return (seq_t)46;         /* second term */

    seq_t a_nm2 = (seq_t)21; /* a(n-2) */
    seq_t a_nm1 = (seq_t)46; /* a(n-1) */
    seq_t a_n;

    for (idx_t i = 3ul; i <= n; ++i) {
        a_n = (seq_t)3 * a_nm1 - (seq_t)2 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
