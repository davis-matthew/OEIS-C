/*
 * A392331  a(n) = 3^n*(4*n+1).
 * https://oeis.org/A392331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n*(4*n+1)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t power = 1;
    for (idx_t i = 0; i < n; ++i)
        power *= (seq_t)3;
    seq_t factor = (seq_t)(4 * (seq_t)n + 1);
    return power * factor;
}
