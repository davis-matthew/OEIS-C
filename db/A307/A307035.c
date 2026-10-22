/*
 * A307035  a(n) is the unique integer k such that A108951(k) = n!.
 * https://oeis.org/A307035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! / ∏_{k=1..n} A064989(k) Equivalent construction: write
 *            n! as a product of primorials p# then replace each p# by its
 *            base prime p. The algorithm below computes the exponents of each
 *            prime in the factorial, solves for the exponents of the
 *            primorials by backward subtraction, and finally bu...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 41, u128 n <= 79,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* ---------- 1. Sieve primes up to n ---------- */
    /* Allocate and initialize sieve array */
    bool *is_prime = (bool *)malloc((n + 1) * sizeof(bool));
    if (!is_prime)
        return (seq_t)0;  /* allocation failure */

    for (idx_t i = 0; i <= n; ++i)
        is_prime[i] = true;
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;

    for (idx_t i = 2; i * i <= n; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= n; j += i)
                is_prime[j] = false;

    /* Collect primes into an array */
    uint32_t *primes = (uint32_t *)malloc((n + 1) * sizeof(uint32_t));
    if (!primes) {
        free(is_prime);
        return (seq_t)0;
    }
    size_t m = 0;                 /* number of primes */
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i])
            primes[m++] = (uint32_t)i;

    free(is_prime);   /* sieve not needed further */

    /* ---------- 2. Compute exponents of each prime in n! ---------- */
    idx_t *exps = (idx_t *)malloc(m * sizeof(idx_t));
    if (!exps) {
        free(primes);
        return (seq_t)0;
    }
    for (size_t i = 0; i < m; ++i) {
        idx_t p = primes[i];
        idx_t e = 0;
        idx_t power = p;
        while (power <= n) {
            e += n / power;
            /* Avoid overflow of power *= p */
            if (power > n / p)
                break;
            power *= p;
        }
        exps[i] = e;
    }

    /* ---------- 3. Backward subtraction to get primorial exponents c_p ---------- */
    idx_t *coeffs = (idx_t *)malloc(m * sizeof(idx_t));
    if (!coeffs) {
        free(primes);
        free(exps);
        return (seq_t)0;
    }
    idx_t suffix = 0;
    for (size_t i = m; i > 0; --i) {
        size_t idx = i - 1;
        coeffs[idx] = exps[idx] - suffix;
        suffix += coeffs[idx];
    }

    /* ---------- 4. Build result: product of p^{c_p} ---------- */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < m; ++i) {
        if (coeffs[i] == 0)
            continue;
        seq_t prime_val = (seq_t)primes[i];
        seq_t pow_val = seq_pow(prime_val, coeffs[i]);
        result = seq_mul(result, pow_val);
    }

    /* Clean up */
    free(primes);
    free(exps);
    free(coeffs);

    return result;
}
