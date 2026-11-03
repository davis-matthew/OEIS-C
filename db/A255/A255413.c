/*
 * A255413  a(n) = 15*n - 11 + (n mod 2). Row 3 of Ludic array A255127.
 * https://oeis.org/A255413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*n - 11 + (n mod 2)
 * Range:     u8 n <= 17, u16 n <= 4369, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 84 terms through n = 9850 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n * 15) - 11 + (seq_t)(n & 1);
}
