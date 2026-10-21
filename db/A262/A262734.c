/*
 * A262734  Period 16: repeat (1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3,
 *          2).
 * https://oeis.org/A262734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( (n % 16) < 9 ) ? ( (n % 16) + 1 ) : ( 17 - (n % 16) )
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = (seq_t)(n % 16);
    if (r < 9)
        return r + 1;
    return (seq_t)17 - r;
}
