/*
 * A364224  Expansion of Sum_{k>=0} 7^k * x^(7^k) / (1 - x^(7^k))^2.
 * https://oeis.org/A364224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n, if n mod 7 ≠ 0; a(n) = n + 7 * a(n/7), otherwise
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset, but safe */
    if (n % 7 == 0)
        return (seq_t)n + (7 * a(n / 7));
    return (seq_t)n;
}
