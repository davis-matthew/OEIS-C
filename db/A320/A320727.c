/*
 * A320727  a(n) is the minimal number of perfect matchings of a graph with 2n
 *          vertices that contains exactly three disjoint perfect matchings.
 * https://oeis.org/A320727
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = n + 1, except a(6) = 6 (the unique known exception)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 6u)          /* special case */
        return (seq_t)6;
    return (seq_t)n + 1;  /* general rule */
}
