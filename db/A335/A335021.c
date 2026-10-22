/*
 * A335021  a(n) = Sum_{d|n, 1 < d < n} (-1)^(d + 1).
 * https://oeis.org/A335021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{d|n, 1<d<n} (-1)^(d+1) (number of odd nontrivial
 *            divisors minus number of even nontrivial divisors)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1u)
        return 0;

    seq_t sum = 0;

    for (idx_t i = 2; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            seq_t term1 = (seq_t)((i % 2 == 0) ? -1 : 1);
            sum += term1;

            /* divisor d2 = n / i, may equal d1 for perfect squares */
            idx_t d2 = n / i;
            if (d2 != i) {
                seq_t term2 = (seq_t)((d2 % 2 == 0) ? -1 : 1);
                sum += term2;
            }
        }
    }

    return sum;
}
