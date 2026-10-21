/*
 * A245339  Sum of digits of n written in fractional base 10/9.
 * https://oeis.org/A245339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of digits of n when represented in base 10/9
 * Range:     u8 n <= 1137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t value = (seq_t)n;
    seq_t sum   = 0;
    while (value != 0) {
        seq_t digit = value % 10;
        sum += digit;
        value = ((value - digit) / 10) * 9;
    }
    return sum;
}
