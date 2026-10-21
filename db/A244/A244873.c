/*
 * A244873  Number of magic labelings of the prism graph I X C_7 with magic
 *          sum n.
 * https://oeis.org/A244873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (488*n^7 + 5124*n^6 + 23408*n^5 + 60270*n^4 + 94472*n^3 *
 *            + 90216*n^2 + 48672*n + 45*(-1)^n + 11475) / 11520. * The
 *            constant term is 11520 for even n and 11430 for odd n
 * Range:     u8 none, u16 n <= 0, u32 n <= 8, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)488;
    N = N * (seq_t)n + (seq_t)5124;
    N = N * (seq_t)n + (seq_t)23408;
    N = N * (seq_t)n + (seq_t)60270;
    N = N * (seq_t)n + (seq_t)94472;
    N = N * (seq_t)n + (seq_t)90216;
    N = N * (seq_t)n + (seq_t)48672;
    /* constant part: 11520 if n even, 11430 if n odd */
    if (n & (idx_t)1) {
        N = N * (seq_t)n + (seq_t)11430;
    } else {
        N = N * (seq_t)n + (seq_t)11520;
    }
    return N / (seq_t)11520;
}
