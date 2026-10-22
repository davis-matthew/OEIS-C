/*
 * A355989  a(n) = n! / (2 * floor(n/2)!).
 * https://oeis.org/A355989
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! / (2 * floor(n/2)!)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined for n < 2, but harmless */
    idx_t m = n / 2;              /* floor(n/2) */
    seq_t prod = 1;
    for (idx_t i = m + 1; i <= n; ++i)
        prod *= (seq_t)i;         /* multiply from floor(n/2)+1 up to n */
    return prod / 2;              /* exact division by 2 */
}
