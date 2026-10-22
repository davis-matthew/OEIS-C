/*
 * A395031  a(n) = 14*n^3 + 28*n^2 + 12*n + 1.
 * https://oeis.org/A395031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*n^3 + 28*n^2 + 12*n + 1
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 673, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* Horner's rule: ((14*N + 28)*N + 12)*N + 1 */
    seq_t res = (((14 * N + 28) * N + 12) * N + 1);
    return res;
}
