/*
 * A287835  Number of words of length n over the alphabet {0,1,...,10} such
 *          that no two consecutive terms have distance 4.
 * https://oeis.org/A287835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=11, a(2)=107, a(3)=1043,
 *            a(n)=10*a(n-1)-a(n-2)-14*a(n-3) for n>3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small indices explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)107;
    if (n == 3) return (seq_t)1043;

    /* We will keep the last three values:
       a_nm3 = a(n-3), a_nm2 = a(n-2), a_nm1 = a(n-1)   */
    seq_t a_nm3 = (seq_t)11;     /* a(1)   */
    seq_t a_nm2 = (seq_t)107;    /* a(2)   */
    seq_t a_nm1 = (seq_t)1043;   /* a(3)   */
    seq_t a_n   = 0;

    for (idx_t i = 4; i <= n; ++i) {
        a_n   = (seq_t)10 * a_nm1 - a_nm2 - (seq_t)14 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
