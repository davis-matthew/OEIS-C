/*
 * A342369  If n is congruent to 2 (mod 3), then a(n) = (2*n - 1)/3;
 *          otherwise, a(n) = 2*n.
 * https://oeis.org/A342369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   if n % 3 == 2 then a(n) = (2*n - 1)/3 else a(n) = 2*n
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n % 3 == 2)
        return ((seq_t)n * 2 - 1) / 3;
    return (seq_t)n * 2;
}
