/*
 * A355819  Dirichlet inverse of A270419, denominator of the rational number
 *          obtained when the exponents in prime factorization of n are
 *          reinterpreted as alternating binary sums (A065620).
 * https://oeis.org/A355819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is the Dirichlet inverse of A270419(n) (multiplicative).
 *            a(1) = 1; for n > 1: a(n) = - Σ_{d|n, d<n} A270419(n/d) · a(d)
 * Range:     i8 n <= 511, i16 n <= 9619, i32 n <= 9619, i64 n <= 9619,
 *            i128 n <= 9619, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The arithmetic function A270419 is multiplicative, with
   A270419(p^e) = p^h(e),  where     h(e) = { 0              if e ≤ 2
                                                e−2           if e is odd and e≥3
                                                max(e−4,0)    if e is even and e≥4 }.
   Consequently the Dirichlet inverse a is multiplicative as well.
   For each prime power p^k we compute the table a(p^0…p^k) by the
   recurrence a(p^k) = - Σ_{i=0}^{k−1} A270419(p^{k−i}) · a(p^i). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow_seq.h"          /* pow_seq(seq_t a, int k) */

/* Small prime list sufficient for factorisation up to 10 000
   (sqrt(10000)=100). */
static const uint64_t SMALL_PRIMES[] = {
    2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
    19ULL,23ULL,29ULL,31ULL,37ULL,41ULL,43ULL,
    47ULL,53ULL,59ULL,61ULL,67ULL,71ULL,73ULL,
    79ULL,83ULL,89ULL,97ULL
};
static const size_t N_SMALL_PRIMES = sizeof(SMALL_PRIMES)/sizeof(SMALL_PRIMES[0]);

/* Compute A270419(p^e).  The exponent of p in the value is
   given by the rule described in the comment above. */
static seq_t
a270419_prime_power(uint64_t p, size_t e)
{
    int exp;                       /* exponent of p in the value */
    if (e <= 2) {
        exp = 0;
    } else if (e & 1) {            /* odd e >= 3 */
        exp = (int)e - 2;
    } else {                       /* even e >= 4 */
        exp = (int)e - 4;
        if (exp < 0) exp = 0;
    }
    return pow_seq((seq_t)p, exp);
}

/* Compute a(p^k) for a prime p and exponent k >= 0.
   a(0) = 1.  The recurrence is applied for k >= 1. */
static seq_t
a_prime_power(uint64_t p, size_t k)
{
    seq_t a[16];          /* max exponent needed for n <= 10000 is < 16 */
    a[0] = (seq_t)1;
    for (size_t m = 1; m <= k; ++m) {
        seq_t sum = (seq_t)0;
        for (size_t i = 0; i < m; ++i) {
            size_t exp_f = m - i;                     /* exponent for A270419 */
            seq_t f = a270419_prime_power(p, exp_f);
            sum += f * a[i];
        }
        a[m] = -sum;
    }
    return a[k];
}

/* Main function required by the evaluator. */
seq_t
a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t remaining = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* trial division using the small prime list */
    for (size_t pi = 0; pi < N_SMALL_PRIMES; ++pi) {
        uint64_t p = SMALL_PRIMES[pi];
        if (p * p > remaining)
            break;
        if (remaining % p == 0) {
            size_t e = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++e;
            }
            result *= a_prime_power(p, e);
        }
    }

    /* If anything remains, it is a prime (greater than sqrt(original n)) */
    if (remaining > 1) {
        /* remaining is a prime factor with exponent 1 */
        result *= a_prime_power(remaining, 1);
    }

    return result;
}
