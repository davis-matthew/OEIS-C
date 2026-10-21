/*
 * A296420  Period of last digit of multiples of n.
 * https://oeis.org/A296420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 1 if n%10==0, else 10 / gcd(n%10,10)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[10] = {1,10,5,10,5,2,5,10,5,10};
    return tbl[(unsigned)(n % 10)];
}
