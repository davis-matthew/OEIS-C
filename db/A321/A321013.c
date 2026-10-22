/*
 * A321013  a(n) = how many of {6,7,8} divide n.
 * https://oeis.org/A321013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = how many of {6,7,8} divide n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    if ((n % 6) == 0) res++;
    if ((n % 7) == 0) res++;
    if ((n % 8) == 0) res++;
    return res;
}
