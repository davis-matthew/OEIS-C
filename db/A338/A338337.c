/*
 * A338337  Coefficient of x^(6*n)*y^(6*n)*z^(6*n) in the expansion of
 *          1/(1-x-y^2-z^3).
 * https://oeis.org/A338337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (11*n)! / ((2*n)! * (3*n)! * (6*n)!)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

/* Helper: exponent of prime p in m! (Legendre's formula) */
static idx_t prime_exp(idx_t p, idx_t m)
{
    idx_t exp = 0;
    idx_t power = p;
    while (power <= m) {
        exp += m / power;
        if (power > m / p) break;
        power *= p;
    }
    return exp;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Upper bound for the sieve: all primes needed are <= 11*n */
    idx_t limit = 11 * n;
    size_t sz = (size_t)limit + 1;

    uint8_t *is_prime = (uint8_t*)malloc(sz);
    if (!is_prime) return (seq_t)0;          /* out of memory */
    for (size_t i = 0; i < sz; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* Count primes and store them */
    size_t primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++primes_cnt;

    uint64_t *primes = (uint64_t*)malloc(primes_cnt * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        return (seq_t)0;
    }

    size_t pidx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[pidx++] = i;
    free(is_prime);

    /* Compute the result using prime factorisation */
    seq_t result = (seq_t)1;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];

        /* Exponents in 11*n! */
        idx_t exp = prime_exp(p, 11 * n);

        /* Subtract exponents in (2*n)!, (3*n)!, (6*n)! */
        exp -= prime_exp(p, 2 * n);
        exp -= prime_exp(p, 3 * n);
        exp -= prime_exp(p, 6 * n);

        if (exp > 0) {
            seq_t pp = seq_pow((seq_t)p, exp);
            result = seq_mul(result, pp);
        }
    }

    free(primes);
    return result;
}
