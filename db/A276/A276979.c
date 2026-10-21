/*
 * A276979  a(n) = (floor(n/2)+1)^n.
 * https://oeis.org/A276979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (floor(n/2)+1)^n
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base = floor(n/2)+1, computed in seq_t arithmetic */
    seq_t base = ((seq_t)n / 2) + 1;

    /* compute power by repeated multiplication */
    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i)
        result *= base;

    return result;
}
