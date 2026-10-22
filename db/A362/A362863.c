/*
 * A362863  Centered hecatonicosachoral numbers.
 * https://oeis.org/A362863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 300*n^4 - 600*n^3 + 420*n^2 - 120*n + 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n3 * N;
    seq_t res = (seq_t)300 * n4
                - (seq_t)600 * n3
                + (seq_t)420 * n2
                - (seq_t)120 * N
                + 1;
    return res;
}
