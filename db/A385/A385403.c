/*
 * A385403  Minimum number of triples that cover {1..n}, such that every
 *          2-coloring of {1..n} results in at least one monochromatic triple.
 * https://oeis.org/A385403
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 10 for n = 5 or 6; * a(n) = 7 + floor((n-5)/3) for n >= 7
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 7) /* n == 5 or 6 (no solutions for n < 5) */
        return (seq_t)10;
    /* for n >= 7 */
    return (seq_t)7 + (seq_t)((n - (idx_t)5) / (idx_t)3);
}
