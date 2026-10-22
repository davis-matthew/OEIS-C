/*
 * A363629  Expansion of Sum_{k>0} (1/(1+x^k)^2 - 1).
 * https://oeis.org/A363629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{d|n} (-1)^d * (d+1)
 * Range:     i8 n <= 63, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n = 0, return 0 */
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {          /* d is a divisor of n */
            if (d & 1U)           /* odd divisor */
                sum -= (seq_t)(d + 1);
            else
                sum += (seq_t)(d + 1);

            idx_t e = n / d;       /* complementary divisor */
            if (e != d) {
                if (e & 1U)
                    sum -= (seq_t)(e + 1);
                else
                    sum += (seq_t)(e + 1);
            }
        }
    }
    return sum;
}
