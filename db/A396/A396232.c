/*
 * A396232  The smallest amount which cannot be made with fewer than n Indian
 *          coins.
 * https://oeis.org/A396232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 (n=1), 3 (n=2), 8 (n=3), 20*n - 62 for n >= 4
 * Range:     u8 n <= 15, u16 n <= 3279, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)8;
    /* n >= 4 */
    return (seq_t)20 * (seq_t)n - (seq_t)62;
}
