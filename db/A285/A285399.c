/*
 * A285399  Start with a single cell at coordinates (0, 0, 0), then
 *          iteratively subdivide the grid into 3 X 3 X 3 cells and remove the
 *          cells whose sum of modulo 2 coordinates is 0 or 2; a(n) is the
 *          number of cells after n iterations.
 * https://oeis.org/A285399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 13, a(n) = 14*a(n-1) for n > 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* equivalent to a(n) = 13*14^(n-1) for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t result = (seq_t)13;
    /* Multiply by 14 for each additional iteration after the first */
    for (idx_t i = 1; i < n; ++i)
        result *= (seq_t)14;
    return result;
}
