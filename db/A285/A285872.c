/*
 * A285872  a(n) is the number of zeros of the Chebyshev S(n, x) polynomial
 *          (A049310) in the open interval (-sqrt(3), +sqrt(3)).
 * https://oeis.org/A285872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*b(n) if n is even, 1 + 2*b(n) if n is odd, where b(n) =
 *            floor(n/2) - floor((n+1)/6)
 * Range:     u8 n <= 257, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;
    seq_t half = sn / 2;
    seq_t div6 = (sn + 1) / 6;
    seq_t b    = half - div6;

    if (n & 1U)          /* odd */
        return b * 2 + 1;
    else                 /* even */
        return b * 2;
}
