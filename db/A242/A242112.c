/*
 * A242112  a(n) = floor((2*n+6)/(5-(-1)^n)).
 * https://oeis.org/A242112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n/2 (n even), a(n) = 1 + floor(n/3) (n odd)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1)
        return (seq_t)(1 + (n / 3));
    else
        return (seq_t)(1 + (n / 2));
}
