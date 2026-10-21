/*
 * A267141  Number of weeks in n! seconds, for n >= 10.
 * https://oeis.org/A267141
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*n!/10! (for n ≥ 10)
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 17, u64 n <= 24, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < (idx_t)10)
        return (seq_t)0;   /* undefined for n < 10 */

    seq_t result = (seq_t)6;          /* 6*(10!/10!) = 6 */
    for (idx_t i = (idx_t)11; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}
