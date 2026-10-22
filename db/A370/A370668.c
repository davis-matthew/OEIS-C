/*
 * A370668  Expansion of Sum_{k>0} k! * ( x * (1+x^k) )^k.
 * https://oeis.org/A370668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} d! * binomial(d, n/d - 1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the provided factorial and binomial utilities. */
#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            idx_t d1 = i;
            seq_t term1 = factorial(d1) * binomial(d1, n / d1 - 1);
            sum += term1;

            /* paired divisor d2 = n / i, if distinct */
            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t term2 = factorial(d2) * binomial(d2, n / d2 - 1);
                sum += term2;
            }
        }
    }
    return sum;
}
