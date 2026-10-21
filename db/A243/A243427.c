/*
 * A243427  Floored (rational) values of sqrt(xy) such that sqrt(x) + sqrt(y)
 *          = sqrt(xy).
 * https://oeis.org/A243427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)^2 / n) = n + 2 for n > 1, a(1) = 4
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* the sequence is defined for n≥1; return 0 as a safe default */ 
        return (seq_t)0;
    if (n == 1)
        return (seq_t)4;
    return (seq_t)n + (seq_t)2;
}
