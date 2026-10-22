/*
 * A367700  Number of degree 2 vertices in the n-Menger sponge graph.
 * https://oeis.org/A367700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*a(n-1) - 244*a(n-2) + 480*a(n-3), with a(1)=12,
 *            a(2)=72, a(3)=744
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* handle the first three terms directly */
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)72;
    if (n == 3) return (seq_t)744;

    /* iterate the linear recurrence for n >= 4 */
    seq_t a_nm3 = (seq_t)12;   /* a(n-3)  */
    seq_t a_nm2 = (seq_t)72;   /* a(n-2)  */
    seq_t a_nm1 = (seq_t)744;  /* a(n-1)  */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        /* compute a(i) = 31*a[i-1] - 244*a[i-2] + 480*a[i-3] */
        cur = (seq_t)31 * a_nm1;
        cur -= (seq_t)244 * a_nm2;
        cur += (seq_t)480 * a_nm3;
        /* shift the window */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return a_nm1;
}
