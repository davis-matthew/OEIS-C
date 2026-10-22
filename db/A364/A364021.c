/*
 * A364021  Expansion of Sum_{k>0} k * x^(3*k) / (1 + x^(5*k)).
 * https://oeis.org/A364021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -Sum_{d|n, n/d==3 (mod 5)} (-1)^(n/d) * d
 * Range:     i8 n <= 344, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t i;
    /* Enumerate divisors by iterating up to sqrt(n) */
    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* First divisor in the pair */
            idx_t d1 = i;
            idx_t e1 = n / d1;          /* exponent corresponding to d1 */
            if (e1 % 5 == 3) {
                seq_t sign = (e1 % 2 == 0) ? 1 : -1;
                sum += -sign * (seq_t)d1;
            }

            /* Second divisor in the pair, if distinct */
            idx_t d2 = n / i;
            if (d2 != d1) {
                idx_t e2 = n / d2;      /* this is equal to d1 */
                if (e2 % 5 == 3) {
                    seq_t sign = (e2 % 2 == 0) ? 1 : -1;
                    sum += -sign * (seq_t)d2;
                }
            }
        }
    }
    return sum;
}
