/*
 * A399490  Sum of squares of the elements of R(n), where R(n) is the set of n
 *          consecutive integers centered at n, with the center n omitted when
 *          n is even.
 * https://oeis.org/A399490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(13*n^2 - 1)/12 for n odd; a(n) = n*(13*n^2 + 3*n +
 *            2)/12 for n even;
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 798, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    if (n & 1) { /* odd */
        return nn * (((seq_t)13 * nn * nn) - (seq_t)1) / (seq_t)12;
    } else { /* even */
        return nn * (((seq_t)13 * nn * nn) + (seq_t)3 * nn + (seq_t)2) / (seq_t)12;
    }
}
