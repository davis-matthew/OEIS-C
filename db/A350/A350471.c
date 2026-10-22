/*
 * A350471  The number of days elapsed since the Gregorian (proleptic) date
 *          Sunday, December 31, 1 BC on 1/1/n, where 1/1/n is the Gregorian
 *          date in the format month/day/year, the New Year's Day of the year
 *          n.
 * https://oeis.org/A350471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 365*(n-1) + floor((n-1)/4) - floor((n-1)/100) +
 *            floor((n-1)/400)
 * Range:     u8 n <= 1, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 (offset 1) */
    seq_t m = (seq_t)n - (seq_t)1;
    seq_t res = (seq_t)1
                + (seq_t)365 * m
                + m / (seq_t)4
                - m / (seq_t)100
                + m / (seq_t)400;
    return res;
}
