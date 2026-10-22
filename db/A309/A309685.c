/*
 * A309685  Number of even parts appearing among the smallest parts of the
 *          partitions of n into 3 parts.
 * https://oeis.org/A309685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{even j=2..floor(n/3)} ((n-j)/2 - j + 1)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The number of partitions of n into three parts with an even smallest part
       can be summed directly from the characterization of the triples (a,b,c)
       with 1 ≤ a ≤ b ≤ c and a+b+c = n.  For each even smallest part a=j,
       the second part b ranges from j to ⌊(n-j)/2⌋, contributing
       ⌊(n-j)/2⌋ - j + 1 partitions. */
    idx_t max_j = n / 3;              /* largest possible smallest part */
    seq_t  res   = 0;

    for (idx_t j = 2; j <= max_j; j += 2)
    {
        seq_t term = ((seq_t)(n - j) / 2) - (seq_t)j + 1;
        res += term;
    }
    return res;
}
