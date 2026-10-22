/*
 * A351605  a(n) = n^7 * Sum_{d^2|n} 1 / d^7.
 * https://oeis.org/A351605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n^7 * Sum_{d^2|n} 1 / d^7 (OEIS A351605)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 565,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Helper: compute p^e for any seq_t type, e is idx_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp != 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Helper: floor integer square root of an idx_t value */
static idx_t isqrt(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t low = 1;
    idx_t high = n / 2 + 1;
    idx_t mid;
    while (low <= high)
    {
        mid = low + ((high - low) >> 1);
        idx_t s = mid * mid;
        if (s == n)
            return mid;
        if (s < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Compute the prime‑power contribution for a term p^e */
static seq_t prime_power_contrib(uint64_t p, unsigned int e)
{
    seq_t pseq     = (seq_t)p;
    seq_t p_pow7   = pow_seq(pseq, 7);
    unsigned int f = (e - 1) / 2;

    seq_t num_e   = pow_seq(pseq, 7 * (idx_t)e);
    seq_t num_f   = pow_seq(pseq, 7 * (idx_t)f);
    seq_t numerator   = num_e - num_f;
    seq_t denominator = p_pow7 - (seq_t)1;

    /* Exact division – the value is guaranteed to be integer */
    seq_t part = (numerator / denominator) * p_pow7;
    return part;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0; return 0 safely */

    seq_t result = 1;
    uint64_t temp = (uint64_t)n;

    /* Factorization by trial division */
    for (uint64_t p = 2; p * p <= temp; p += (p == 2 ? 1 : 2))
    {
        if (temp % p == 0)
        {
            unsigned int e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                e++;
            }
            result *= prime_power_contrib(p, e);
        }
    }

    /* Remaining prime factor greater than sqrt(original n) */
    if (temp > 1)
        result *= prime_power_contrib(temp, 1);

    return result;
}
