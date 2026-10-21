/*
 * A264234  Numerators of the coefficients in the expansion of 1/W(x) - 1/x
 *          where W(x) is the Lambert W function.
 * https://oeis.org/A264234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = (-1)^n * numerator(n^n / n!)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           = (-1)^n * product_{p prime <= n} p^max(0, n*e_p(n) - sum_{k>=1} floor(n/p^k)) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    /* -------------------------------------*
     *  1) Compute list of primes <= n       *
     * ------------------------------------- */
    static uint32_t *prime_list = NULL;
    static size_t prime_count = 0;
    static size_t max_computed = 0;

    if (n > max_computed) {
        size_t limit = (size_t)n + 1;
        bool *sieve = (bool*)malloc(limit);
        if (!sieve) return (seq_t)0;       /* allocation failure – return 0 */

        /* simple Eratosthenes */
        for (size_t i = 0; i < limit; ++i) sieve[i] = true;
        sieve[0] = sieve[1] = false;
        for (size_t i = 2; i * i < limit; ++i) {
            if (sieve[i]) {
                for (size_t j = i * i; j < limit; j += i)
                    sieve[j] = false;
            }
        }
        /* count primes */
        size_t count = 0;
        for (size_t i = 2; i < limit; ++i) if (sieve[i]) ++count;
        /* reallocate */
        uint32_t *new_list = (uint32_t*)realloc(prime_list, count * sizeof(uint32_t));
        if (!new_list) { free(sieve); return (seq_t)0; }
        prime_list = new_list;
        prime_count = 0;
        for (size_t i = 2; i < limit; ++i)
            if (sieve[i]) prime_list[prime_count++] = (uint32_t)i;
        free(sieve);
        max_computed = n;
    }

    /* -------------------------------------*
     *  2) Compute the numerator part        *
     * ------------------------------------- */
    seq_t numerator = (seq_t)1;

    for (size_t ip = 0; ip < prime_count; ++ip) {
        uint32_t p = prime_list[ip];
        if (p > n) break;

        /* exponent of p in n */
        uint32_t tmp = (uint32_t)n;
        uint32_t e_n = 0;
        while (tmp % p == 0) {
            ++e_n;
            tmp /= p;
        }
        if (e_n == 0) continue;  /* p does not divide n */

        seq_t exp_p_n_pow = (seq_t)n * (seq_t)e_n;   /* exponent of p in n^n */

        /* sum_{k>=1} floor(n / p^k) */
        seq_t sum_floor = 0;
        uint32_t pk = p;
        while ((uint64_t)pk <= n) {
            sum_floor += n / pk;
            /* avoid overflow in pk *= p */
            if (pk > n / p) break;
            pk *= p;
        }

        seq_t exp_gcd = (sum_floor < exp_p_n_pow) ? sum_floor : exp_p_n_pow;
        seq_t exp_num = exp_p_n_pow - exp_gcd;      /* exponent in the reduced numerator */

        if (exp_num > 0) {
            seq_t p_pow = big_pow((seq_t)p, exp_num);
            numerator *= p_pow;
        }
    }

    /* -------------------------------------*
     *  3) Apply sign (-1)^n                 *
     * ------------------------------------- */
    if (n & 1) {
        numerator = -numerator;
    }

    return numerator;
}
