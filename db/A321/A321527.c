/*
 * A321527  Expansion of x^3 * c(x^2) * c(x^4)^2 / (9 * c(x)) in powers of x
 *          where c() is a cubic AGM theta function.
 * https://oeis.org/A321527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = s(n/3) - s(n/4) - s(n/6) + s(n/12) where s(k) is the sum
 *            of divisors of k (0 if k is not a positive integer)
 * Range:     i8 n <= 269, i16 n <= 9309, i32 n <= 9309, i64 n <= 9309,
 *            i128 n <= 9309, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: sum of positive divisors of k (k ≥ 0). */
static seq_t sigma(idx_t k)
{
    if (k == 0) return 0;
    seq_t sum = 0;
    for (idx_t i = 1; i <= k / i; ++i) {
        if (k % i == 0) {
            sum += i;
            idx_t j = k / i;
            if (j != i) sum += j;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t s3 = (n % 3 == 0) ? sigma(n / 3) : 0;
    seq_t s4 = (n % 4 == 0) ? sigma(n / 4) : 0;
    seq_t s6 = (n % 6 == 0) ? sigma(n / 6) : 0;
    seq_t s12 = (n % 12 == 0) ? sigma(n / 12) : 0;
    return s3 - s4 - s6 + s12;
}
