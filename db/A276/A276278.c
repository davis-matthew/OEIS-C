/*
 * A276278  Complement of A026474.
 * https://oeis.org/A276278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 3; for n ≥ 2, a(n) = n + ceil((n+2)/6) + 2 (equivalently
 *            a(n) = n + (n+7)/6 + 2 using integer division)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)3;
    seq_t m = (seq_t)n;
    return m + ((m + (seq_t)7) / (seq_t)6) + (seq_t)2;
}
