/*
 * A397984  Lower (3/2,1) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A397984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((7*n*n + 3*n)/4)
 * Range:     u8 n <= 5, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t part1 = nn * nn;          /* nn^2      */
    part1 *= (seq_t)7;              /* 7*nn^2    */
    seq_t part2 = nn * (seq_t)3;    /* 3*nn      */
    seq_t sum   = part1 + part2;    /* 7*nn^2+3*nn */
    return sum / (seq_t)4;          /* floor division by 4 */
}
