/*
 * A379222  Number of trailing 1-bits in the binary representation of the sum
 *          of the divisors of the n-th odd square: a(n) = sigma((2*n-1)^2).
 * https://oeis.org/A379222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of trailing 1‑bits in σ((2*n‑1)^2)
 * Range:     u8 n <= 1111, u16 n <= 9176, u32 n <= 9176, u64 n <= 9176,
 *            u128 n <= 9176, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* σ(m) is the sum of all positive divisors of m. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* odd square root */
    seq_t k = (seq_t)n * 2 - 1;

    /* factor k and compute σ(k²) */
    seq_t temp = k;
    seq_t sigma = 1;
    seq_t d = 3;                          /* k is odd → start at 3, step by 2 */

    while (d * d <= temp) {
        if (temp % d == 0) {
            /* exponent of d in k */
            seq_t e = 0;
            while (temp % d == 0) {
                temp /= d;
                ++e;
            }

            /* sum of powers up to d^(2*e) */
            seq_t sum = 1;
            seq_t pwr = 1;
            seq_t limit = e * 2;          /* 2*e iterations */
            for (seq_t i = 0; i < limit; ++i) {
                pwr *= d;
                sum += pwr;
            }
            sigma *= sum;
        }
        d += 2;
    }

    /* remaining prime factor (if any) */
    if (temp > 1) {
        seq_t sum = 1;
        seq_t pwr = 1;
        for (seq_t i = 0; i < 2; ++i) {   /* exponent 2 for the remaining prime */
            pwr *= temp;
            sum += pwr;
        }
        sigma *= sum;
    }

    /* count trailing 1‑bits in σ(k²) */
    seq_t count = 0;
    while ((sigma & 1) == 1) {
        ++count;
        sigma >>= 1;
    }

    return count;
}
