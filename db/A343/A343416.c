/*
 * A343416  a(n) = A001414(n) + A000203(n) + A001414(A000203(n)) +
 *          A000203(A001414(n)).
 * https://oeis.org/A343416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001414(n) + A000203(n) + A001414(A000203(n)) +
 *            A000203(A001414(n)) (sum of prime factors with multiplicity +
 *            sum of divisors + the two cross terms)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*--------------------------------------------------------------------
   Compute A001414(n): sum of prime factors of n with multiplicity.
--------------------------------------------------------------------*/
static seq_t sum_prime_factors(idx_t n)
{
    if (n <= 1U) return (seq_t)0;
    seq_t sum = (seq_t)0;
    idx_t temp = n;
    for (idx_t p = 2U; p <= temp / p; ++p) {
        while (temp % p == 0U) {
            sum += (seq_t)p;
            temp /= p;
        }
    }
    if (temp > 1U) sum += (seq_t)temp;
    return sum;
}

/*--------------------------------------------------------------------
   Compute A000203(n): sum of divisors of n.
--------------------------------------------------------------------*/
static seq_t sigma_of(idx_t n)
{
    if (n == 0U) return (seq_t)0;   /* unused, but defined for completeness */
    if (n == 1U) return (seq_t)1;
    seq_t result = (seq_t)1;
    idx_t temp = n;
    for (idx_t p = 2U; p <= temp / p; ++p) {
        if (temp % p == 0U) {
            /* compute p^e factor */
            idx_t exp = 0U;
            while (temp % p == 0U) {
                temp /= p;
                exp++;
            }
            /* term = 1 + p + p^2 + ... + p^exp */
            seq_t term = (seq_t)1;
            seq_t power = (seq_t)1;
            for (idx_t i = 0U; i < exp; ++i) {
                power *= (seq_t)p;
                term += power;
            }
            result *= term;
        }
    }
    if (temp > 1U) {
        /* remaining prime factor contributes (1 + p) */
        result *= ((seq_t)1 + (seq_t)temp);
    }
    return result;
}

/*--------------------------------------------------------------------
   Main sequence function, exposed with external linkage.
--------------------------------------------------------------------*/
extern seq_t a(idx_t n)
{
    seq_t s_n      = sum_prime_factors(n);       /* A001414(n)   */
    seq_t sigma_n  = sigma_of(n);                /* A000203(n)   */

    /* Convert to idx_t for the two nested calls.  For normal ranges
       of n this fits easily; if the cast would overflow the behaviour
       is implementation defined but will not occur for the intended use. */
    idx_t sigma_n_idx = (idx_t)sigma_n;
    idx_t s_n_idx     = (idx_t)s_n;

    seq_t s_sigma_n  = sum_prime_factors(sigma_n_idx);   /* A001414(A000203(n)) */
    seq_t sigma_s_n  = sigma_of(s_n_idx);               /* A000203(A001414(n)) */

    return s_n + sigma_n + s_sigma_n + sigma_s_n;
}
