/*
 * A373864  Reduced Collatz function R applied to the numbers 6n+5: a(n) =
 *          R(6n+5), where R(k) = (3k+1)/2^r, with r as large as possible.
 * https://oeis.org/A373864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 8) / 2^v2(9*n + 8) (odd part of 3*(6n+5)+1)
 * Range:     u8 n <= 27, u16 n <= 7280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)9 * n + 8;          /* 9*n + 8 */
    while ((x & 1) == 0)                 /* remove powers of two */
        x >>= 1;
    return x;
}
