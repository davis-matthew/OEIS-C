/*
 * A296784  Detour index for the n X n torus grid graph.
 * https://oeis.org/A296784
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^2-1)^2/2 for odd n; a(n) = n^2*(2*n^4-5*n^2+4)/4
 *            for even n
 * Range:     u8 none, u16 n <= 5, u32 n <= 44, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    if ((n & 1) == 1) {
        seq_t n2m1 = n2 - 1;
        seq_t term = n2 * (n2m1 * n2m1);
        return term / 2;
    } else {
        seq_t n4 = n2 * n2;
        seq_t expr = 2 * n4 - 5 * n2 + 4;
        seq_t term = n2 * expr;
        return term / 4;
    }
}
