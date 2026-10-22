/*
 * A322927  Expansion of x*(1 + 5*x + 40*x^2)/((1 - x^2)*(1 - 10*x^2)).
 * https://oeis.org/A322927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number consisting of n/2 copies of digit 5 for even n,
 *            and for odd n the number consists of (n-1)/2 copies of 5
 *            followed by 1
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 18, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t res = 0;
    if ((n & 1) == 0) {               /* even n */
        idx_t d = n / 2;
        for (idx_t i = 0; i < d; ++i)
            res = res * 10 + 5;
    } else {                           /* odd n */
        idx_t d = (n + 1) / 2;          /* total digits */
        for (idx_t i = 0; i < d - 1; ++i)
            res = res * 10 + 5;
        res = res * 10 + 1;
    }
    return res;
}
