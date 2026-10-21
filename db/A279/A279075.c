/*
 * A279075  Maximum starting value of X such that repeated replacement of X
 *          with X-ceiling(X/5) requires n steps to reach 0.
 * https://oeis.org/A279075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(a(n-1)*5/4) + 1 for n > 0, a(0) = 0 * * The
 *            recurrence can be rewritten without multiplication overflow as *
 *            a(n) = a(n-1) + floor(a(n-1)/4) + 1 * * This yields all terms of
 *            OEIS A279075 (Maximum starting value of X * such that repeated
 *            replacement of X with X - ceil(X/5) requires n...
 * Range:     u8 n <= 20, u16 n <= 45, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t val = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i)
    {
        val = val + (val / (seq_t)4) + (seq_t)1;
    }
    return val;
}
