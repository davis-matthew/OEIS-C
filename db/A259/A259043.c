/*
 * A259043  a(n) = if n < 10 then n else a(floor(n/10) + n mod 10) + n mod 10.
 * https://oeis.org/A259043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n for n < 10 a(n) = a(floor(n/10)+n mod 10) + n mod 10
 *            for n >= 10 Initial values: a(0)=0, a(1)=1, ..., a(9)=9
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 10)
        return (seq_t)n;
    idx_t x = n / 10;
    idx_t d = n % 10;
    return a(x + d) + (seq_t)d;
}
