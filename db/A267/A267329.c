/*
 * A267329  Number of nX(n+1) arrays of permutations of n+1 copies of 0..n-1
 *          with every element equal to or 1 greater than any southwest or
 *          northwest neighbors modulo n and the upper left element equal to
 *          0.
 * https://oeis.org/A267329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n=1; a(n) = 10 for n=2; a(n) = 15 for n=3; a(n) =
 *            8*n - 10 for n >= 4
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)15;
    /* for n >= 4: 8*n - 10 */
    seq_t res = ((seq_t)n << 3) - (seq_t)10;
    return res;
}
