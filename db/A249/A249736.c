/*
 * A249736  Triangular numbers modulo 30.
 * https://oeis.org/A249736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 mod 30
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence has period 60, so reduce n modulo 60 first. */
    idx_t m = n % 60;
    idx_t t = m * (m + 1) / 2;   /* fits comfortably in idx_t */
    return (seq_t)(t % 30);
}
