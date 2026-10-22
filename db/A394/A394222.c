/*
 * A394222  a(n) = n * gcd(5, n).
 * https://oeis.org/A394222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n * gcd(5, n)
 * Range:     u8 n <= 54, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n % 5 == 0)
        return ((seq_t)n) * 5;
    else
        return (seq_t)n;
}
