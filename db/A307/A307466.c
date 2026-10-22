/*
 * A307466  Number of Catalan words of length n avoiding the pattern 210.
 * https://oeis.org/A307466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(4)=14,
 *            a(n)=6*a(n-1)-11*a(n-2)+5*a(n-3)+2*a(n-4) for n≥5
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)14;

    seq_t a_nm4 = (seq_t)1;  /* a(1)      */
    seq_t a_nm3 = (seq_t)2;  /* a(2)      */
    seq_t a_nm2 = (seq_t)5;  /* a(3)      */
    seq_t a_nm1 = (seq_t)14; /* a(4)      */
    seq_t a_cur = 0;

    for (idx_t i = 5; i <= n; ++i) {
        a_cur = ( (seq_t)6 * a_nm1 )
              - ( (seq_t)11 * a_nm2 )
              + ( (seq_t)5 * a_nm3 )
              + ( (seq_t)2 * a_nm4 );

        /* shift window */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }

    return a_cur;
}
