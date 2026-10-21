/*
 * A283444  a(n) = lcm(n,7) / gcd(n,7).
 * https://oeis.org/A283444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n % 7 == 0) ? n/7 : 7*n; a(0) = 0, a(1) = 7, a(2) = 14,
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n % 7) == 0)
        return (seq_t)n / 7;
    else
        return (seq_t)n * 7;
}
