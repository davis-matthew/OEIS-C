/*
 * A382524  Number of ways to choose a different constant partition of each
 *          part of a constant partition of n.
 * https://oeis.org/A382524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} binomial(A000005(n/d), d) * d!
 * Range:     u8 n <= 47, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   binomial, divisor_count_u64, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n) {
    /* a(0) = 1 by definition */
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t sum = (seq_t)0;

    /* iterate over all divisors d of n */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* first divisor d */
            idx_t m1 = n / d;
            seq_t tau1 = divisor_count_u64(m1);
            sum += binomial((idx_t)tau1, d) * factorial(d);

            /* second divisor n/d if different */
            idx_t d2 = n / d;
            if (d2 != d) {
                idx_t m2 = n / d2;
                seq_t tau2 = divisor_count_u64(m2);
                sum += binomial((idx_t)tau2, d2) * factorial(d2);
            }
        }
    }
    return sum;
}
