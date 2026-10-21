/*
 * A242604  a(n) = (n - 1)*(n^3 + 1) = n^4 - n^3 + n - 1, for n >= 1.
 * https://oeis.org/A242604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 1)*(n^3 + 1) = n^4 - n^3 + n - 1
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t N = (seq_t)n;
    seq_t cube = N * N * N;
    seq_t res = (N - 1) * (cube + 1);
    return res;
}
