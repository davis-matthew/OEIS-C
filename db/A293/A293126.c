/*
 * A293126  Number of matchings in the 2n-crossed prism graph.
 * https://oeis.org/A293126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 18*a(n-2) + 4*a(n-3), a(0) = 3, a(1) = 12,
 *            a(2) = 108
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* handle the initial cases */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)108;

    /* a(n-3), a(n-2), a(n-1) */
    seq_t a_nm3 = (seq_t)3;
    seq_t a_nm2 = (seq_t)12;
    seq_t a_nm1 = (seq_t)108;
    seq_t a_i;

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 12*a(i-1) - 18*a(i-2) + 4*a(i-3) */
        a_i = (seq_t)12 * a_nm1 - (seq_t)18 * a_nm2 + (seq_t)4 * a_nm3;
        /* shift for next iteration */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_i;
    }
    return a_nm1;
}
