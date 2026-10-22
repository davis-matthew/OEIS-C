/*
 * A326826  a(n) = (1/2) * Sum_{d|n} (sigma_1(d) + sigma_2(d)), where sigma_1
 *          = A000203 and sigma_2 = A001157.
 * https://oeis.org/A326826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (1/2) * Sum_{d|n} (sigma_1(d) + sigma_2(d))
 * Range:     u8 n <= 11, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes the sigma₁ and sigma₂ of every divisor
   of n by direct enumeration, summing and halving the result.     */

#include <stddef.h>
#include <stdbool.h>

/* Helper to evaluate σ₁(m) and σ₂(m) at once. */
static void sigma1_sigma2(idx_t m, seq_t *s1, seq_t *s2) {
    seq_t sum1 = 0, sum2 = 0;
    for (idx_t k = 1; k <= m / k; ++k) {
        if (m % k == 0) {
            seq_t other = m / k;
            sum1 += (seq_t)k;
            sum2 += (seq_t)k * (seq_t)k;
            if (k != other) {
                sum1 += other;
                sum2 += (seq_t)other * (seq_t)other;
            }
        }
    }
    *s1 = sum1;
    *s2 = sum2;
}

/* Main OEIS sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for the offset, but harmless */
    idx_t N = n;
    seq_t total = 0;

    for (idx_t d = 1; d <= N / d; ++d) {
        if (N % d == 0) {
            seq_t s1, s2;
            sigma1_sigma2(d, &s1, &s2);
            total += (s1 + s2);

            idx_t d2 = N / d;
            if (d != d2) {
                sigma1_sigma2(d2, &s1, &s2);
                total += (s1 + s2);
            }
        }
    }

    return total / 2; /* exact division, result is always integer */
}
