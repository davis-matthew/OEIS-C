/*
 * A301601  Numbers k such that k^6 can be written as a sum of 11 positive 6th
 *          powers.
 * https://oeis.org/A301601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 18 a(2) = 19 a(3) – a(7) = n + 27 a(n ≥ 8) = n + 28 This
 *            implements the sequence of all integers ≥ 18 except 20–29 and
 *            35, which is the same as OEIS A301601
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2u)
        return (seq_t)n + 17u;
    if (n <= 7u)
        return (seq_t)n + 27u;
    return (seq_t)n + 28u;
}
