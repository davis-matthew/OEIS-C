/*
 * A271912  Number of ways to choose three distinct points from a 3 X n grid
 *          so that they form an isosceles triangle.
 * https://oeis.org/A271912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=10, for n>2: if n is even: a(n) = (5*n^2 + 32*n -
 *            72)/2 if n is odd : a(n) = (5*n^2 + 32*n - 69)/2
 * Range:     u8 n <= 5, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)10;

    seq_t nn = (seq_t)n;
    seq_t term = (seq_t)5 * nn * nn + (seq_t)32 * nn;
    if ((nn & (seq_t)1) == 0) {          /* even */
        term -= (seq_t)72;
    } else {                             /* odd */
        term -= (seq_t)69;
    }
    return term / (seq_t)2;
}
