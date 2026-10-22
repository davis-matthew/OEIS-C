/*
 * A326038  Square root of the largest square dividing the sum of divisors of
 *          n: a(n) = A000188(sigma(n)).
 * https://oeis.org/A326038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = √(largest square dividing σ(n)) = A000188(σ(n)) , n ≥ 1
 *            where σ(n) is the sum of positive divisors of n
 * Range:     u8 n <= 2212, u16 n <= 9962, u32 n <= 9962, u64 n <= 9962,
 *            u128 n <= 9962, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small helper: binary exponentiation for seq_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 1. Compute σ(n) – sum of positive divisors of n */
    seq_t sigma = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            sigma += (seq_t)i;
            idx_t d = n / i;
            if (d != i)
                sigma += (seq_t)d;
        }
    }

    /* 2. Find the largest square divisor of sigma and return its sqrt */
    seq_t result = 1;
    seq_t rem = sigma;

    for (idx_t p = 2; (seq_t)p * (seq_t)p <= rem; ++p) {
        seq_t p_seq = (seq_t)p;
        idx_t exp = 0;
        while (rem % p_seq == 0) {
            rem /= p_seq;
            ++exp;
        }
        if (exp > 0) {
            idx_t half = exp / 2;
            if (half > 0)
                result *= pow_seq(p_seq, half);
        }
    }

    /* Any remaining prime factor of rem has exponent 1 – it contributes no square factor. */
    return result;
}
