/*
 * A279079  Maximum starting value of X such that repeated replacement of X
 *          with X-ceiling(X/9) requires n steps to reach 0.
 * https://oeis.org/A279079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(a(n-1)*9/8) + 1, a(0) = 0 (OEIS A279079)
 * Range:     u8 n <= 33, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* a(0) = 0 */
        return 0;

    seq_t prev = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* floor division is integral truncation for positives */
        prev = (prev * 9 / 8) + 1;
    }
    return prev;
}
