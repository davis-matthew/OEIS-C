/*
 * A241200  For the n in A241199, the index of the first of 4 terms in
 *          binomial(n,k) that satisfy a quadratic relation.
 * https://oeis.org/A241200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-11-5*(-1)^n-2*(-15+(-1)^n)*n+6*n^2)/16 Piecewise: a(n)
 *            = (3*n*n +14*n -8)/8 if n even, a(n) = (3*n*n +16*n -3)/8 if n
 *            odd
 * Range:     u8 n <= 23, u16 n <= 415, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    if (n & 1) { /* n is odd */
        /* a(n) = (3*n*n +16*n -3)/8 */
        return ((seq_t)3 * nn * nn + (seq_t)16 * nn - (seq_t)3) / (seq_t)8;
    } else {     /* n is even */
        /* a(n) = (3*n*n +14*n -8)/8 */
        return ((seq_t)3 * nn * nn + (seq_t)14 * nn - (seq_t)8) / (seq_t)8;
    }
}
