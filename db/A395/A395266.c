/*
 * A395266  a(n) = a(n-1) + a(n-3) + a(n-4), a(0) = 1, a(1) = 5, a(2) = 25,
 *          a(3) = 30.
 * https://oeis.org/A395266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4) (a(0)=1, a(1)=5, a(2)=25,
 *            a(3)=30)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the initial values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)25;
    if (n == 3) return (seq_t)30;

    /* Keep the last four values in a rolling window */
    seq_t a0 = (seq_t)1;   /* a(i-4)  */
    seq_t a1 = (seq_t)5;   /* a(i-3)  */
    seq_t a2 = (seq_t)25;  /* a(i-2)  */
    seq_t a3 = (seq_t)30;  /* a(i-1)  */
    seq_t res;

    for (idx_t i = 4; i <= n; ++i) {
        res = a3 + a1 + a0;     /* a(i) = a(i-1) + a(i-3) + a(i-4) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = res;
    }

    return a3;  /* a(n) */
}
