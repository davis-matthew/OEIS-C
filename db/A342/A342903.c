/*
 * A342903  a(n) is the smallest number that is the sum of n positive squares
 *          in two ways.
 * https://oeis.org/A342903
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 15 for n >= 5 with initial special cases: a(2) = 50
 *            a(3) = 27 a(4) = 28 Values for n < 2 are undefined and return 0
 * Range:     u8 n <= 240, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 2) return (seq_t)50;
    if (n == 3) return (seq_t)27;
    if (n == 4) return (seq_t)28;
    if (n >= 5) return (seq_t)n + (seq_t)15;
    return (seq_t)0;   /* undefined for n < 2 */
}
