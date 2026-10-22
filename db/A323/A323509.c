/*
 * A323509  a(n) = 1 if n = 2^e1 * 3^e2 * ... * prime(k)^e_k, with k =
 *          A061395(n) = A001221(n) and e1 >= e2 >= ... >= e_k, with each e1
 *          .. e_k one less than a prime, otherwise a(n) = 0.
 * https://oeis.org/A323509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n uses the first k primes as its prime factors, with
 *            exponents e1 ≥ e2 ≥ … ≥ ek, each exponent ei + 1 is prime;
 *            otherwise 0
 * Range:     u8 n <= 9762, u16 n <= 9762, u32 n <= 9762, u64 n <= 9762,
 *            u128 n <= 9762, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

static const uint64_t first_primes[15] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47
};

seq_t a(idx_t n)
{
    /* Special case n = 1 */
    if (n == (idx_t)1) {
        return (seq_t)1;
    }

    uint64_t primes[20];   /* sufficient for any 64‑bit number */
    uint64_t exps[20];
    size_t   cnt = 0;

    /* Factor n into primes and exponents: primes[] is ascending */
    factor_u64(n, primes, exps, &cnt);

    /* Check that the prime list matches the first cnt primes */
    if (cnt > 15) {          /* impossible for 64‑bit n but keep safe */
        return (seq_t)0;
    }
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] != first_primes[i]) {
            return (seq_t)0;
        }
    }

    /* Exponents must be non‑increasing (e1 ≥ e2 ≥ …) */
    for (size_t i = 0; i + 1 < cnt; ++i) {
        if (exps[i] < exps[i + 1]) {
            return (seq_t)0;
        }
    }

    /* Each exponent + 1 must be a prime */
    for (size_t i = 0; i < cnt; ++i) {
        if (!is_prime((idx_t)(exps[i] + 1))) {
            return (seq_t)0;
        }
    }

    return (seq_t)1;
}
