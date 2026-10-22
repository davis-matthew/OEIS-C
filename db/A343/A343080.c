/*
 * A343080  a(n) is the smallest number that is the sum of n positive squares
 *          in three ways.
 * https://oeis.org/A343080
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 325 for n=2; 54 for n=3; n+24 for n>=4
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 2) return (seq_t)325;
    if (n == 3) return (seq_t)54;
    if (n >= 4) return (seq_t)n + (seq_t)24;
    return (seq_t)0;   /* for n < 2 the offset is not defined */
}
