/*
 * A325941  Expansion of Sum_{k>=1} k * x^(2*k) / (1 + x^k)^2.
 * https://oeis.org/A325941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} (-1)^(n/d) * (n - d)
 * Range:     i8 n <= 31, i16 n <= 1439, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but guard anyway */
    seq_t total = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* first divisor d1 = i */
            idx_t d1   = i;
            idx_t exp1 = n / d1;                 /* n/d1 */
            seq_t term = (seq_t)n - (seq_t)d1;
            total += (exp1 & 1) ? -term : term;   /* sign depends on parity of exp1 */

            /* second divisor d2 = n/i, avoid duplicate when i*i == n */
            idx_t d2 = n / i;
            if (d2 != d1) {
                idx_t exp2 = n / d2;               /* this is i */
                seq_t term2 = (seq_t)n - (seq_t)d2;
                total += (exp2 & 1) ? -term2 : term2;
            }
        }
    }
    return total;
}
