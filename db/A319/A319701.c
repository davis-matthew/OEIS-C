/*
 * A319701  Filter sequence for sequences that are constant for all odd terms
 *          >= 3.
 * https://oeis.org/A319701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(2) = 2; for n > 2, if n is odd, a(n) = 3; otherwise
 *            a(n) = 2 + n/2
 * Range:     u8 n <= 507, u16 n <= 9242, u32 n <= 9242, u64 n <= 9242,
 *            u128 n <= 9242, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n & (idx_t)1) return (seq_t)3;
    return (seq_t)2 + (seq_t)(n / 2);
}
