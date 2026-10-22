/*
 * A390874  Molecular topological index of the n-necklace graph.
 * https://oeis.org/A390874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*(13 + (-1)^n + 12*n^2) (offset 1)
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 12*n^2 + 13 ± 1, where ±1 is +1 if n is even, -1 if odd */
    seq_t term = (seq_t)12 * n * n + (seq_t)13;
    if (n & (idx_t)1)          /* odd */
        term -= (seq_t)1;
    else                       /* even */
        term += (seq_t)1;

    return (seq_t)3 * n * term;
}
