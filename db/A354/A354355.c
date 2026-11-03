/*
 * A354355  Characteristic function of numbers with their sum of divisors
 *          (sigma) 3-smooth.
 * https://oeis.org/A354355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if sigma(n) is 3‑smooth (product of powers of 2 and 3),
 *            else 0
 * Range:     u8 n <= 107, u16 n <= 100000, u32 n <= 100000, u64 n <= 100000,
 *            u128 n <= 100000, big any n
 * Validated: standard, pass, 143 terms through n = 9764 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    seq_t sigma = sigma_of(n);
    /* Remove all factors of 3 */
    while ((sigma % 3) == 0) sigma /= 3;
    /* Remove all factors of 2 */
    while ((sigma % 2) == 0) sigma /= 2;
    /* If remainder is 1, sigma was 3‑smooth */
    return (sigma == 1) ? 1 : 0;
}
