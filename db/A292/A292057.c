/*
 * A292057  Wiener index of the n X n queen graph.
 * https://oeis.org/A292057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 1)*n*(3*n^2 - 2*n + 1)/3
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t n1   = nn - (seq_t)1;
    seq_t term = (seq_t)3 * nn * nn - (seq_t)2 * nn + (seq_t)1;
    seq_t num  = n1 * nn;
    num        = num * term;
    return num / (seq_t)3;
}
