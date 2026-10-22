/*
 * A345037  a(n) = Sum_{k=1..n} (-k)^(floor(n/k) - 1).
 * https://oeis.org/A345037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (-k)^(floor(n/k) - 1)
 * Range:     i8 n <= 14, i16 n <= 31, i32 n <= 62, i64 n <= 122,
 *            i128 n <= 245, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t e = n / k - 1;            /* exponent floor(n/k)-1  */
        seq_t base = -(seq_t)k;         /* (-k) */
        seq_t term = 1, b = base;
        while (e) {
            if (e & 1ULL) term *= b;    /* multiply when bit is set */
            b *= b;                     /* square base */
            e >>= 1;
        }
        sum += term;
    }
    return sum;
}
