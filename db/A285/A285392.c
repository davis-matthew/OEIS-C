/*
 * A285392  Start with a single cell at coordinates (0, 0), then iteratively
 *          subdivide the grid into 3 X 3 cells and remove the cells whose sum
 *          of modulo 2 coordinates is 0; a(n) is the number of cells after n
 *          iterations.
 * https://oeis.org/A285392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=5, a(2)=36, a(n)=9*a(n-1)-12*a(n-2) for n≥3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)36;

    /* Iterate the recurrence from n=3 up to the requested n. */
    seq_t prev2 = (seq_t)5;      /* a(1)   */
    seq_t prev1 = (seq_t)36;     /* a(2)   */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i)
    {
        cur = (seq_t)9 * prev1 - (seq_t)12 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
