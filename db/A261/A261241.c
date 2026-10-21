/*
 * A261241  One half of numbers representable in at least two different ways
 *          as sums of four nonvanishing cubes. See A259060 for these numbers
 *          and their representations.
 * https://oeis.org/A261241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+9)*(2*n^2 + 36*n + 357)
 * Range:     u8 none, u16 n <= 21, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the sequence integer type */
    seq_t nn = (seq_t)n;

    /* Compute the formula using only integral arithmetic */
    seq_t part1 = nn + (seq_t)9;
    seq_t part2 = (seq_t)2 * nn * nn + (seq_t)36 * nn + (seq_t)357;

    return part1 * part2;
}
