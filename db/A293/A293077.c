/*
 * A293077  Number of letters (0's and 1's) in the n-th iterate of the
 *          final-letter-removed mapping defined at A289035.
 * https://oeis.org/A293077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*floor(a(n-2)/4) (a(1)=2, a(2)=4)
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n < 1, return 0 */

    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)4;

    seq_t a_nm2 = (seq_t)2;   /* a(1) */
    seq_t a_nm1 = (seq_t)4;   /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t term = (seq_t)2 * a_nm1 - a_nm2 + (seq_t)2 * (a_nm2 / (seq_t)4);
        a_nm2 = a_nm1;
        a_nm1 = term;
    }
    return a_nm1;
}
