/*
 * A270059  Number of distinct digits needed to write n in all bases >= 2.
 * https://oeis.org/A270059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor((n + 3) / 2) for n >= 3; a(0) = 1, a(1) = 1, a(2) =
 *            3
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u || n == 1u)
        return (seq_t)1;
    if (n == 2u)
        return (seq_t)3;
    return ((seq_t)n + (seq_t)3) / (seq_t)2;
}
