/*
 * A287926  Sum of the divisors of n that are not divisible by 49.
 * https://oeis.org/A287926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 3082, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A287926
   Formula: a(n) = Σ_{d|n, 49∤d} d
   Multiplicative: a(7^e) = 8 (for e≥1), a(p^e) = (p^(e+1)-1)/(p-1) otherwise */

#include <stdint.h>

/* Helper: integer power with seq_t base and int exponent */
static seq_t power_uint(seq_t base, int exp)
{
    seq_t result = 1;
    for (; exp > 0; exp >>= 1, base *= base)
        if (exp & 1) result *= base;
    return result;
}

/* Helper: sum of divisors of a value n (idx_t), result in seq_t */
static seq_t sigma_value(idx_t n)
{
    seq_t sum = 1;
    idx_t temp = n;

    /* factor 2 */
    if (temp % 2 == 0) {
        int e = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            e++;
        }
        seq_t term = (power_uint((seq_t)2, e + 1) - 1) / ((seq_t)2 - 1);
        sum *= term;
    }

    /* odd factors */
    for (idx_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            seq_t term = (power_uint((seq_t)p, e + 1) - 1) / ((seq_t)p - 1);
            sum *= term;
        }
    }

    /* remaining prime factor */
    if (temp > 1) {
        seq_t term = (power_uint((seq_t)temp, 2) - 1) / ((seq_t)temp - 1);
        sum *= term;
    }

    return sum;
}

seq_t a(idx_t n)
{
    /* Count exponent of 7 in n */
    idx_t temp = n;
    int exp7 = 0;
    while (temp % 7 == 0) {
        temp /= 7;
        exp7++;
    }

    if (exp7 < 2) {
        /* all divisors are allowed */
        return sigma_value(n);
    } else {
        /* only divisors with 7-exponent 0 or 1 are allowed;
           sum = 8 * sigma(m) where m = n / 7^exp7 */
        seq_t sigma_m = sigma_value(temp);
        return sigma_m * (seq_t)8;
    }
}
