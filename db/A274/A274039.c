/*
 * A274039  Expansion of (x^4 + x^10) / (1 - 2*x + x^2).
 * https://oeis.org/A274039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n < 4; a(n) = n - 3 for 4 ≤ n < 9; a(n) = 2 * (n -
 *            6) for n ≥ 9
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;
    if (n < 9) return (seq_t)(n - 3);
    return (seq_t)(n - 6) * 2;
}
