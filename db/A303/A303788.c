/*
 * A303788  a(n) = Sum_{i=0..m} d(i)*5^i, where Sum_{i=0..m} d(i)*6^i is the
 *          base-6 representation of n.
 * https://oeis.org/A303788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=0}^{m} d(i)*5^i, where n = Σ d(i)*6^i
 * Range:     u8 n <= 402, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t pow    = 1;
    idx_t x      = n;
    while (x > 0)
    {
        seq_t digit = x % 6;         /* base‑6 digit (0‑5) */
        result += digit * pow;
        pow *= 5;
        x /= 6;
    }
    return result;
}
