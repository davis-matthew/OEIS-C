/*
 * A285395  Start with a single cell at coordinates (0, 0, 0), then
 *          iteratively subdivide the grid into 3 X 3 X 3 cells and remove the
 *          cells whose sum of modulo 2 coordinates is 1; a(n) is the number
 *          of cells after n iterations.
 * https://oeis.org/A285395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*a(n-1) - 195*a(n-2) + 324*a(n-3), a(0) = 1, a(1) = 15,
 *            a(2) = 249
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)15;
    if (n == 2) return (seq_t)249;

    seq_t a0 = (seq_t)1;   /* a(n-3)  */
    seq_t a1 = (seq_t)15;  /* a(n-2)  */
    seq_t a2 = (seq_t)249; /* a(n-1)  */

    seq_t val;
    for (idx_t i = 3; i <= n; ++i) {
        val = (seq_t)28 * a2;
        val -= (seq_t)195 * a1;
        val += (seq_t)324 * a0;

        a0 = a1;
        a1 = a2;
        a2 = val;
    }
    return a2;
}
