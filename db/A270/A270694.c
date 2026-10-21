/*
 * A270694  Alternating sum of centered heptagonal pyramidal numbers.
 * https://oeis.org/A270694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = ((-1)^n*(2*n + 1)*(14*n^2 + 14*n - 9) + 9)/48
 * Range:     i8 n <= 1, i16 n <= 10, i32 n <= 444, i64 n <= 500,
 *            i128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;
    seq_t two_n_plus1 = nseq * (seq_t)2 + (seq_t)1;
    seq_t n2 = nseq * nseq;
    seq_t t1 = (seq_t)14 * n2 + (seq_t)14 * nseq - (seq_t)9;
    seq_t product = two_n_plus1 * t1;
    seq_t sign = (n & (idx_t)1) ? -(seq_t)1 : (seq_t)1;
    seq_t numerator = sign * product + (seq_t)9;
    return numerator / (seq_t)48;
}
