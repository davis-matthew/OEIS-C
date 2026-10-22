/*
 * A307295  If n is even, a(n) = A001950(n/2+1), otherwise a(n) =
 *          A001950((n-1)/2+1) + 1.
 * https://oeis.org/A307295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(((n/2)+1)*φ²) + (n mod 2) where φ² = (3 + √5) / 2 ≈
 *            2.618033988749894848204586
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* k = n/2 + 1  (k is an integer ≥ 1) */
    idx_t k = n / 2 + 1;
    /* long double gives sufficient precision for k up to about 10⁴ */
    static const long double PHI2 = 2.618033988749894848204586L;
    long double prod = (long double)k * PHI2;      /* product */
    /* truncation toward zero is equivalent to floor for positive numbers */
    unsigned long long base = (unsigned long long)prod;
    seq_t result = (seq_t)base + (seq_t)(n & 1ULL);
    return result;
}
