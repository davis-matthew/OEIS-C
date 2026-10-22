/*
 * A334694  a(n) = (n/4)*(n^3+2*n^2+5*n+8).
 * https://oeis.org/A334694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^3+2*n^2+5*n+8)/4
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 271, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t t  = n3 + (seq_t)2 * n2 + (seq_t)5 * nn + (seq_t)8;
    seq_t num = nn * t;
    seq_t res = num / (seq_t)4;
    return res;
}
