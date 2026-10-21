/*
 * A285397  Start with a single cell at coordinates (0, 0, 0), then
 *          iteratively subdivide the grid into 3 X 3 X 3 cells and remove the
 *          cells whose sum of modulo 2 coordinates is 3; a(n) is the number
 *          of cells after n iterations.
 * https://oeis.org/A285397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*a(n-1) - 195*a(n-2) + 216*a(n-3), a(0)=1, a(1)=26,
 *            a(2)=646
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)26;
    if (n == 2) return (seq_t)646;

    seq_t a0 = (seq_t)1;      /* a(0) */
    seq_t a1 = (seq_t)26;     /* a(1) */
    seq_t a2 = (seq_t)646;    /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = ((seq_t)32 * a2) - ((seq_t)195 * a1) + ((seq_t)216 * a0);
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;
}
