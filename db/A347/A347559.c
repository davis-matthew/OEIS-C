/*
 * A347559  Number of minimum dominating sets in the n-Moebius ladder.
 * https://oeis.org/A347559
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9 for n=3; a(n) = n * (n + 2) when n % 4 == 0, a(n) = 2 *
 *            n when n is odd and n > 3, a(n) = 4 when n % 4 == 2
 * Range:     u8 n <= 15, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3)
        return 9;
    if ((n & 3) == 0)   /* n % 4 == 0 */
        return (seq_t)n * ((seq_t)n + 2);
    if ((n & 1) == 1)   /* odd */
        return 2 * (seq_t)n;
    return (seq_t)4;    /* n % 4 == 2 */
}
