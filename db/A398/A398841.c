/*
 * A398841  Upper (1, 3/2)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((11*n^2 - n)/4)
 * Range:     u8 n <= 4, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to the element type to avoid loss of precision */
    seq_t nn = (seq_t)n;
    seq_t prod = nn * nn;                       /* n^2 */
    seq_t x = 11 * prod - nn;                   /* 11*n^2 - n */
    /* ceil division by 4: (x + 3) / 4 for non‑negative x */
    return (x + 3) / 4;
}
