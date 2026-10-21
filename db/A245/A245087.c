/*
 * A245087  Largest number such that 2^a(n) is a divisor of (n!)!.
 * https://oeis.org/A245087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v2((n!)!) = sum_{k>=1} ⌊(n!)/2^k⌋ where v2(m) is the
 *            exponent of 2 in m!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n! as a big integer in seq_t */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= i;

    /* compute exponent of 2 in (n!)! via the standard sum of floor division */
    seq_t result = 0;
    seq_t temp = fact;
    while (temp > 0)
    {
        temp >>= 1;      /* temp = ⌊temp / 2⌋ */
        result += temp;
    }
    return result;
}
