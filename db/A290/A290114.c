/*
 * A290114  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 643", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 3 For n > 1: a(n) = 3*2^(n-1) - 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    /* n > 1 */
    seq_t pow2 = ((seq_t)1 << (n - 1));   /* 2^(n-1) */
    seq_t result = (seq_t)3 * pow2 - (seq_t)1;
    return result;
}
