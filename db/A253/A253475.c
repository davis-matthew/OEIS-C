/*
 * A253475  Indices of centered square numbers (A001844) which are also
 *          centered hexagonal numbers (A003215).
 * https://oeis.org/A253475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=6, a(3)=55, recurrence: a(n)=11*a(n-1) - 11*a(n-2)
 *            + a(n-3) (n>=4)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)55;

    seq_t a_nm3 = (seq_t)1;   /* a(n-3)  */
    seq_t a_nm2 = (seq_t)6;   /* a(n-2)  */
    seq_t a_nm1 = (seq_t)55;  /* a(n-1)  */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t diff = a_nm1 - a_nm2;        /* a(n-1) - a(n-2)  */
        cur = 11 * diff + a_nm3;          /* 11*(a(n-1)-a(n-2)) + a(n-3) */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return cur;
}
