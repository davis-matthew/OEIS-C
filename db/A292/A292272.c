/*
 * A292272  a(n) = n - A048735(n) = n - (n AND floor(n/2)).
 * https://oeis.org/A292272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n - (n AND floor(n/2))
 * Range:     u8 n <= 255, u16 n <= 9448, u32 n <= 9448, u64 n <= 9448,
 *            u128 n <= 9448, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor(n/2) == n >> 1 */
    idx_t m = n & (n >> 1);
    return (seq_t)(n - m);
}
