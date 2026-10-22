/*
 * A363616  Expansion of Sum_{k>0} x^(4*k)/(1+x^k)^4.
 * https://oeis.org/A363616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} (-1)^d * binomial(d-1,3)
 * Range:     i8 n <= 11, i16 n <= 59, i32 n <= 2231, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    if (n == 0) return res;     /* not used (offset is 1) */

    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            if (d1 >= 4) {
                seq_t term = ((seq_t)(d1 - 1) * (d1 - 2) * (d1 - 3)) / 6;
                res += (d1 & 1) ? -term : term;
            }

            if (d2 != d1 && d2 >= 4) {
                seq_t term = ((seq_t)(d2 - 1) * (d2 - 2) * (d2 - 3)) / 6;
                res += (d2 & 1) ? -term : term;
            }
        }
    }

    return res;
}
