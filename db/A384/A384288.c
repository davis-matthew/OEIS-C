/*
 * A384288  Length of the long leg in the unique primitive Pythagorean triple
 *          whose inradius is A002378(n) and such that its long leg and its
 *          hypotenuse are consecutive natural numbers.
 * https://oeis.org/A384288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(n+1)*(n^2+n+1)
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 214, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = 2 * nn * (nn + 1) * (nn * nn + nn + 1);
    return val;
}
