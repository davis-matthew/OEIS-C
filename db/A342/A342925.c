/*
 * A342925  a(n) = A003415(sigma(n)), where A003415 is the arithmetic
 *          derivative, and sigma is the sum of divisors of n.
 * https://oeis.org/A342925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(sigma(n))
 * Range:     u8 n <= 41, u16 n <= 4619, u32 n <= 9684, u64 n <= 9684,
 *            u128 n <= 9684, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Compute the arithmetic derivative of an integer given its prime factors.
   The function is inlined for clarity. */

seq_t a(idx_t n) {
    /* --- Fast prime sieve (static) ------------------------------------- */
    static uint64_t *primes = NULL;
    static size_t p_cnt = 0;
    const idx_t PRIME_LIMIT = 200000;          /* plenty for n <= 10000    */

    if (primes == NULL) {
        size_t sz = (size_t)PRIME_LIMIT + 1;
        bool *is_comp = (bool*)malloc(sz);
        memset(is_comp, 0, sz);
        primes = (uint64_t*)malloc(sz * sizeof(uint64_t));
        p_cnt = 0;

        for (idx_t i = 2; i <= PRIME_LIMIT; ++i) {
            if (!is_comp[i]) {
                primes[p_cnt++] = i;
                if (i * i <= PRIME_LIMIT) {
                    for (idx_t j = i * i; j <= PRIME_LIMIT; j += i)
                        is_comp[j] = true;
                }
            }
        }
        free(is_comp);
    }

    /* --- Compute sigma(n) ---------------------------------------------- */
    seq_t sigma = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            sigma += (seq_t)i;
            idx_t j = n / i;
            if (j != i)
                sigma += (seq_t)j;
        }
    }

    /* --- Arithmetic derivative of sigma --------------------------------- */
    seq_t der = 0;
    uint64_t tmp = (uint64_t)sigma;          /* work with 64-bit */
    for (size_t pi = 0; pi < p_cnt && primes[pi] * primes[pi] <= tmp; ++pi) {
        if (tmp % primes[pi] == 0) {
            unsigned exp = 0;
            while (tmp % primes[pi] == 0) {
                tmp /= primes[pi];
                exp++;
            }
            seq_t term = sigma / (seq_t)primes[pi];
            der += term * (seq_t)exp;
        }
    }
    if (tmp > 1) {                            /* remaining prime factor   */
        seq_t term = sigma / (seq_t)tmp;
        der += term;        /* exp == 1 */
    }

    return der;
}
