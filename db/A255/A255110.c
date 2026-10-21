/*
 * A255110  Number of length n+3 0..2 arrays with at most one downstep in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^5 + 30*n^4 + 295*n^3 + 2790*n^2 + 5284*n + 1320) / 120
 * Range:     u8 none, u16 n <= 3, u32 n <= 82, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t n5 = n4 * nn;
    seq_t num = n5 + (seq_t)30 * n4 + (seq_t)295 * n3
                + (seq_t)2790 * n2 + (seq_t)5284 * nn + (seq_t)1320;
    return num / (seq_t)120;
}
