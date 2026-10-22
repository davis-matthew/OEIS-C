/*
 * A336319  Squarefree part of n*d(n), where d(n) = number of divisors of n
 *          (A000005).
 * https://oeis.org/A336319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007913(n*A000005(n)) = squarefree part of n*d(n)
 * Range:     u8 n <= 130, u16 n <= 8604, u32 n <= 8604, u64 n <= 8604,
 *            u128 n <= 8604, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Factor n */
    size_t cnt_n = 0;
    uint64_t primes_n[64];
    uint64_t exps_n[64];
    factor_u64((uint64_t)n, primes_n, exps_n, &cnt_n);

    /* Compute d(n) = product (e_i+1) */
    seq_t d = 1;
    for (size_t i = 0; i < cnt_n; i++) {
        d *= (seq_t)(exps_n[i] + 1);
    }

    /* Factor d(n) */
    size_t cnt_d = 0;
    uint64_t primes_d[64];
    uint64_t exps_d[64];
    factor_u64((uint64_t)d, primes_d, exps_d, &cnt_d);

    /* Build squarefree part of n*d(n) */
    seq_t result = 1;

    /* Handle primes coming from n */
    for (size_t i = 0; i < cnt_n; i++) {
        uint64_t p = primes_n[i];
        uint64_t e = exps_n[i];

        /* Add exponent from d if present */
        for (size_t j = 0; j < cnt_d; j++) {
            if (primes_d[j] == p) {
                e += exps_d[j];
                break;
            }
        }

        if (e & 1ULL) {
            result *= (seq_t)p;
        }
    }

    /* Handle primes that are only in d(t) */
    for (size_t i = 0; i < cnt_d; i++) {
        uint64_t p = primes_d[i];

        /* Skip if already processed from n */
        bool in_n = false;
        for (size_t k = 0; k < cnt_n; k++) {
            if (primes_n[k] == p) {
                in_n = true;
                break;
            }
        }

        if (in_n) continue;

        if (exps_d[i] & 1ULL) {
            result *= (seq_t)p;
        }
    }

    return result;
}
