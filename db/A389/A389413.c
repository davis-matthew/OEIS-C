/*
 * A389413  a(n) is the least integer k such that among the n consecutive
 *          integers k, k + 1, ..., k + n - 1, the sum of any majority subset
 *          exceeds the sum of its complement.
 * https://oeis.org/A389413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-1)^2)/4 + 1 if n is odd a(n) = 0 if n = 2 a(n) =
 *            floor(n*(n-4)/8) + 1 if n is even and n >= 4 (derived from the
 *            OEIS description)
 * Range:     u8 n <= 32, u16 n <= 512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n % 2U == 1U)          /* odd n */
    {
        seq_t m = (seq_t)(n - 1U);
        return (m * m) / 4 + 1;
    }
    else                       /* even n */
    {
        if (n == 2U)
            return 0;
        seq_t m = (seq_t)n;
        return (m * (m - 4)) / 8 + 1;
    }
}
