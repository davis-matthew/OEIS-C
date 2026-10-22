/*
 * A356541  a(n) = Sum_{d|n} d * (d!)^(n/d-1).
 * https://oeis.org/A356541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d * (d!)^(n/d-1) (OEIS A356541)
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 23, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* The type seq_t is provided by the build (uint8_t, uint16_t, ... or arbitrary‑precision).
   The type idx_t is also provided (uint64_t).  All arithmetic should be performed in seq_t. */

/* Compute the factorial of a small integer d (0 ≤ d ≤ n). */
static seq_t factorial(idx_t d)
{
    seq_t f = 1;
    for (idx_t i = 2; i <= d; ++i)
        f *= (seq_t)i;
    return f;
}

/* Binary exponentiation: compute base^exp (exp ≥ 0).  All arithmetic is in seq_t. */
static seq_t power(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* The main sequence function. */
seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* Enumerate all positive divisors d of n. */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        /* Handle divisor d. */
        idx_t k = n / d;              /* k = n/d */
        seq_t term;
        if (k <= 1)                   /* exponent = k-1 = 0 */
            term = (seq_t)d;          /* d * 1 */
        else {
            seq_t fac = factorial(d);
            seq_t pow_val = power(fac, k - 1);
            term = pow_val * (seq_t)d;
        }
        sum += term;

        /* Handle the paired divisor n/d if distinct. */
        idx_t d2 = n / d;              /* d2 = n/d */
        if (d2 != d) {
            idx_t k2 = n / d2;         /* k2 = n/d2 = d */
            seq_t term2;
            if (k2 <= 1)
                term2 = (seq_t)d2;
            else {
                seq_t fac2 = factorial(d2);
                seq_t pow_val2 = power(fac2, k2 - 1);
                term2 = pow_val2 * (seq_t)d2;
            }
            sum += term2;
        }
    }

    return sum;
}
