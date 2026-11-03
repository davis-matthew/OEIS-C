/*
 * A332844  Dirichlet g.f.: zeta(s) * zeta(s-1) * zeta(2*s).
 * https://oeis.org/A332844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = prod_{p^e || n} a(p^e), where a(p^e) = sum_{k=0}^e
 *            f(p^k), and f(p^k) = p^{k-2⌊k/2⌋} * (p^{2(⌊k/2⌋+1)} - 1) / (p^2
 *            - 1)
 * Range:     u8 n <= 24, u16 n <= 12959, u32 n <= 7185, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 81 terms through n = 9155 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires: seq_t and idx_t are defined elsewhere. */

#include <stdbool.h>

/* Compute f(p^e) for given prime p (as seq_t) and exponent e (unsigned int). */
static seq_t f_prime_power(seq_t p, unsigned int exp)
{
    if (exp == 0) return (seq_t)1;
    unsigned int m   = exp / 2;          /* floor(exp/2)          */
    unsigned int rem = exp % 2;          /* 0 or 1                 */
    /* base = p^(rem) */
    seq_t base = (seq_t)1;
    for (unsigned int i = 0; i < rem; ++i) base *= p;

    /* denom = p^2 - 1 */
    seq_t denom = p * p - (seq_t)1;

    /* numerator = p^(2*(m+1)) - 1 */
    seq_t power = (seq_t)1;
    unsigned int pow_exp = 2 * (m + 1);
    for (unsigned int i = 0; i < pow_exp; ++i) power *= p;
    seq_t numerator = power - (seq_t)1;

    return base * numerator / denom;
}

/* Compute a(p^e) for prime p (as seq_t) and exponent e.            */
static seq_t a_prime_power(seq_t p, unsigned int exp)
{
    seq_t sum = (seq_t)1;                     /* starts with f(p^0)=1    */
    for (unsigned int k = 1; k <= exp; ++k)
        sum += f_prime_power(p, k);
    return sum;
}

/* Public function:  seq_t a(idx_t n)   */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;        /* not defined for 0        */
    idx_t rem   = n;
    seq_t result = (seq_t)1;

    /* trial division up to sqrt(rem) */
    for (idx_t p = 2; p * p <= rem; ++p)
    {
        if (rem % p == 0)
        {
            unsigned int exp = 0;
            while (rem % p == 0)
            {
                rem /= p;
                ++exp;
            }
            result *= a_prime_power((seq_t)p, exp);
        }
    }

    /* remaining prime factor, if any */
    if (rem > 1)
        result *= a_prime_power((seq_t)rem, 1);

    return result;
}
