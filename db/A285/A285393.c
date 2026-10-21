/*
 * A285393  Start with a single cell at coordinates (0, 0, 0), then
 *          iteratively subdivide the grid into 3 X 3 X 3 cells and remove the
 *          cells whose sum of modulo 2 coordinates is 2 or 3; a(n) is the
 *          number of cells after n iterations.
 * https://oeis.org/A285393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 48*a(n-2), a(0) = 1, a(1) = 20
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)20;

    seq_t prev2 = (seq_t)1;      /* a(0) */
    seq_t prev1 = (seq_t)20;     /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (seq_t)20 * prev1 - (seq_t)48 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
