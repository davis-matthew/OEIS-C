/*
 * A254619  a(n) = 4^n*(2*n + 1)!/n!.
 * https://oeis.org/A254619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n*(2*n + 1)!/n! Recurrence: a(0) = 1, a(n) = 8*(2*n +
 *            1)*a(n-1) for n >= 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;                /* a(0) = 1 */
    for (idx_t i = 1; i <= n; ++i) {
        /* Compute the multiplier 8*(2*i + 1) in seq_t */
        seq_t mul = (seq_t)(8 * (2 * i + 1));
        result = result * mul;
    }
    return result;
}
