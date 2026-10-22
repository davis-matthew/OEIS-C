/*
 * A394223  a(n) = n * gcd(7, n).
 * https://oeis.org/A394223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n * gcd(7, n)
 * Range:     u8 n <= 41, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n % 7 == 0)
        return (seq_t)n * ((seq_t)7);
    return (seq_t)n;
}
