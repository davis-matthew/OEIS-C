/*
 * A287794  Nine steps forward, eight steps back.
 * https://oeis.org/A287794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} (-1)^floor((2*i-2)/17) Equivalent: a(n) =
 *            (n/17) + (n%17 <= 9 ? (n%17) : (18 - (n%17)))
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t c = n / 17;
    idx_t r = n % 17;
    seq_t add = (r <= 9) ? (seq_t)r : (seq_t)(18 - r);
    return (seq_t)c + add;
}
