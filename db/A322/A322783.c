/*
 * A322783  a(n) = 1 - n + (2^(n+2) - (-1)^n)/3.
 * https://oeis.org/A322783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 - n + (2^(n+2) - (-1)^n)/3. Initial values: a(0)=2,
 *            a(1)=3, a(2)=4, a(3)=9. Recurrence: a(n) = 3*a(n-1) - a(n-2) -
 *            3*a(n-3) + 2*a(n-4) for n > 3
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)9;

    seq_t a_nm4 = (seq_t)2;   /* a(n-4)  */
    seq_t a_nm3 = (seq_t)3;   /* a(n-3)  */
    seq_t a_nm2 = (seq_t)4;   /* a(n-2)  */
    seq_t a_nm1 = (seq_t)9;   /* a(n-1)  */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i) {
        cur = a_nm1 * (seq_t)3;
        cur -= a_nm2;
        cur -= a_nm3 * (seq_t)3;
        cur += a_nm4 * (seq_t)2;

        /* roll the window forward */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return a_nm1;
}
