/*
 * A386469  The largest divisor of n whose exponents in its prime
 *          factorization are squares.
 * https://oeis.org/A386469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e∥n} p^{⌊√e⌋²} (largest divisor of n whose prime
 *            exponents are perfect squares)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, int_sqrt, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"

/* Computes the largest divisor of n whose prime exponents are perfect squares.
   Uses trial division with primes up to sqrt(n). */
seq_t a(idx_t n) {
    /* Handle n = 0 or 1 explicitly */
    if (n <= 1) return (seq_t)1;

    uint64_t temp = (uint64_t)n;
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;

    /* Build all primes up to sqrt(n) */
    int_sqrt(n); /* just to ensure the header is used */
    build_primes_upto(int_sqrt(n), &primes, &primes_cnt);

    seq_t result = (seq_t)1;          /* product accumulator */
    seq_t p_seq;                      /* current prime as seq_t */
    seq_t prod;                       /* prime contribution */

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* largest square <= e */
            int s = 0;
            while ((s + 1) * (s + 1) <= e) ++s;
            int exp = s * s;

            p_seq = (seq_t)p;
            prod = (seq_t)1;
            for (int j = 0; j < exp; ++j) {
                prod = seq_mul(prod, p_seq);
            }
            result = seq_mul(result, prod);
        }
    }

    /* Remaining prime factor > sqrt(original n) */
    if (temp > 1) {
        /* exponent is 1 → largest square <=1 is 1 */
        p_seq = (seq_t)temp;
        result = seq_mul(result, p_seq);
    }

    free(primes);
    return result;
}
