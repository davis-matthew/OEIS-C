/*
 * A255910  Decimal expansion of 16/9.
 * https://oeis.org/A255910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy, less
 * Formula:   a(1) = 1; a(n) = 7 for n > 1
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    else
        return (seq_t)7;
}
