/*
 * A398847  Upper (1/3, 1/2)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*(11*n-1)/12)
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                      /* promote index to seq_t */
    seq_t p  = nn * (((seq_t)11 * nn) - 1);   /* n*(11*n-1) */
    return (p + 11) / 12;                    /* ceil division by 12 */
}
