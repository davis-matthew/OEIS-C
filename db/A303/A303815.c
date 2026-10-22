/*
 * A303815  Generalized 29-gonal (or icosienneagonal) numbers: m*(27*m - 25)/2
 *          with m = 0, +1, -1, +2, -2, +3, -3, ...
 * https://oeis.org/A303815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(27*n + 50)/8 if n is even a(n) = (n + 1)*(27*n - 23)/8
 *            if n is odd
 * Range:     u8 n <= 7, u16 n <= 139, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    if (n == 0)
        return 0;
    if (n & 1U) { /* n is odd */
        return ((sn + 1) * ((27 * sn) - 23)) / 8;
    } else {      /* n is even */
        return (sn * ((27 * sn) + 50)) / 8;
    }
}
