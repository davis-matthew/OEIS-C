/*
 * A350091  a(n) = a(floor(n/4)) for n == 2 (mod 4), otherwise n.
 * https://oeis.org/A350091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n % 4 != 2; otherwise a(n) = a(n/4) where n%4 == 2
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    while ((n & 3u) == 2u) {
        n /= 4;
    }
    return (seq_t)n;
}
