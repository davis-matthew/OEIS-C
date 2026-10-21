/*
 * A290923  p-INVERT of the positive integers, where p(S) = 1 - 2*S - 2*S^2.
 * https://oeis.org/A290923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=6*a(n-1)-8*a(n-2)+6*a(n-3)-a(n-4), a(0)=2, a(1)=10,
 *            a(2)=46, a(3)=208
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)46;
    if (n == 3) return (seq_t)208;

    /* iterative computation of the recurrence; 4 previous terms needed */
    seq_t a_nm4 = (seq_t)2;    /* a(0)   */
    seq_t a_nm3 = (seq_t)10;   /* a(1)   */
    seq_t a_nm2 = (seq_t)46;   /* a(2)   */
    seq_t a_nm1 = (seq_t)208;  /* a(3)   */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t a_n = (seq_t)6 * a_nm1
                  - (seq_t)8 * a_nm2
                  + (seq_t)6 * a_nm3
                  - a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
