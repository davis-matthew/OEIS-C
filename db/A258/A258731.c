/*
 * A258731  Number of length n+1 0..3 arrays with at most one downstep in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A258731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^7 + 35*n^6 + 511*n^5 + 3185*n^4 + 10024*n^3 +
 *            19460*n^2 + 27264*n + 20160)/5040
 * Range:     u8 none, u16 none, u32 n <= 18, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nt   = (seq_t)n;
    seq_t n2  = nt * nt;
    seq_t n3  = n2 * nt;
    seq_t n4  = n3 * nt;
    seq_t n5  = n4 * nt;
    seq_t n6  = n5 * nt;
    seq_t n7  = n6 * nt;

    seq_t num = n7;
    num += 35 * n6;
    num += 511 * n5;
    num += 3185 * n4;
    num += 10024 * n3;
    num += 19460 * n2;
    num += 27264 * nt;
    num += 20160;

    return num / 5040;
}
