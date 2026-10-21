/*
 * A255981  a(n) = greatest number of edges in a simple polygon on an n X n
 *          grid with no two edges parallel.
 * https://oeis.org/A255981
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n*(n-1)/2 + 2
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset is n = 2 */
    seq_t nn = (seq_t)n;
    return (nn * (nn - 1)) / 2 + 2;
}
