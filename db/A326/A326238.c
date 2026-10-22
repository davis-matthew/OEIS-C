/*
 * A326238  Expansion of Sum_{k>=1} k * x^k * (1 - x^k) / (1 + x^k)^3.
 * https://oeis.org/A326238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = n * Sum_{d|n} (-1)^(d+1) * d
 * Range:     i8 n <= 10, i16 n <= 119, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t d2 = n / d;
            seq_t sign_d  = ((d & 1) ? (seq_t)1 : (seq_t)-1);
            sum += (seq_t)d * sign_d;
            if (d2 != d) {
                seq_t sign_d2 = ((d2 & 1) ? (seq_t)1 : (seq_t)-1);
                sum += (seq_t)d2 * sign_d2;
            }
        }
    }
    return (seq_t)n * sum;
}
