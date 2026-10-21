/*
 * A258589  Minimal most likely sum for a roll of n 12-sided dice.
 * https://oeis.org/A258589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(13*n/2)
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 1;          /* a(1) = 1 by definition */
    seq_t half = (seq_t)(n >> 1);  /* n/2 */
    seq_t res  = half * 13;        /* 13*(n/2)  (no overflow if result fits in seq_t) */
    if (n & 1)                     /* add 6 for odd n */
        res += 6;
    return res;
}
