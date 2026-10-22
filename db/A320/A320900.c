/*
 * A320900  Expansion of Sum_{k>=1} x^k/(1 + x^k)^3.
 * https://oeis.org/A320900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = Sum_{d|n} (-1)^(d+1) * d * (d+1) / 2
 * Range:     i8 n <= 14, i16 n <= 215, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    if (n == 0) return res;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            /* sign = +1 for odd d, -1 for even d */
            seq_t sign1 = (d1 & 1) ? 1 : -1;
            seq_t contrib1 = sign1 * ((seq_t)d1 * (seq_t)(d1 + 1) / 2);
            res += contrib1;
            if (d2 != d1) {
                seq_t sign2 = (d2 & 1) ? 1 : -1;
                seq_t contrib2 = sign2 * ((seq_t)d2 * (seq_t)(d2 + 1) / 2);
                res += contrib2;
            }
        }
    }
    return res;
}
