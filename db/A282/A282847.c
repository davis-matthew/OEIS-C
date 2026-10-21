/*
 * A282847  Given n people seated at a table, a(n) is the minimum number of
 *          swaps that must occur in order for everybody to have sat next to
 *          every other person.
 * https://oeis.org/A282847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = ceil(n*(n-3)/8) + (1 if 5 ≤ n ≤ 11 and n ≠ 7)
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 1, 2, 3 the value is 0. */
    if (n < 4)
        return 0;

    /* Compute ceil(n*(n-3)/8) */
    seq_t prod = (seq_t)n * ((seq_t)n - 3);
    seq_t res  = (prod + 7) / 8;   /* ceil division */

    /* Small corrections for n = 5, 6, 8, 9, 10, 11 */
    if (n >= 5 && n <= 11 && n != 7)
        ++res;

    return res;
}
