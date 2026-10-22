/*
 * A340793  Sequence whose partial sums give A000203.
 * https://oeis.org/A340793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = σ(n) − σ(n-1) (σ is the sum‑of‑divisors function)
 * Range:     i8 n <= 63, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute σ(n) = sum of divisors of n. */
static seq_t sigma_of(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t temp = n;

    for (idx_t p = (idx_t)2; p <= temp / p; ++p)
    {
        if (temp % p != 0) continue;

        seq_t p_pow = (seq_t)1;
        seq_t sum    = (seq_t)1;

        while (temp % p == 0) {
            temp /= p;
            p_pow = p_pow * (seq_t)p;
            sum   = sum + p_pow;
        }

        result = result * sum;
    }

    /* If a prime factor greater than sqrt(original n) remains */
    if (temp > 1) {
        seq_t sum = (seq_t)1 + (seq_t)temp;
        result = result * sum;
    }

    return result;
}

/* Public function required by the problem statement.  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* just to keep the function total */
    return sigma_of(n) - sigma_of(n - 1);
}
