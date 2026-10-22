/*
 * A392590  Upper irredundance and domination number of the n-Moebius ladder.
 * https://oeis.org/A392590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n while n mod 4 ≠ 2, otherwise n-1
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* value for indices below the offset is irrelevant */
    /* subtract 1 only when n mod 4 == 2 */
    return (seq_t)(n - ((n & 3) == 2 ? 1 : 0));
}
