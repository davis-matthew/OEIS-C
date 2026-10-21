/*
 * A243579  Integers of the form 8k+7 that can be written as a sum of four
 *          distinct squares of the form m, m+2, m+4, m+5, where m == 1 (mod
 *          4).
 * https://oeis.org/A243579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64*n^2 - 8*n + 15
 * Range:     u8 n <= 2, u16 n <= 32, u32 n <= 8192, u64 n <= 9080,
 *            u128 n <= 9080, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;                 /* promote index to seq_t */
    return ((seq_t)64 * nn * nn)  /* 64*n^2 */
           - ((seq_t)8  * nn) +   /* -8*n   */
           (seq_t)15;            /* +15    */
}
