/*
 * A346234  Dirichlet inverse of A003961, where A003961(n) is fully
 *          multiplicative with a(prime(k)) = prime(k+1).
 * https://oeis.org/A346234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = 0 if n has a squared prime factor; otherwise * a(n) =
 *            (-1)^k * ∏_{p|n} nextprime(p), where k = number of distinct
 *            primes
 * Range:     i8 n <= 41, i16 n <= 4829, i32 n <= 9840, i64 n <= 9840,
 *            i128 n <= 9840, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* factor the integer n */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    /* if any exponent > 1, result is 0 */
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] > 1)
            return (seq_t)0;
    }

    seq_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t nextp = (seq_t)next_prime(primes[i]);
        prod *= nextp;
    }

    if (cnt & 1)               /* odd number of prime factors → negative sign */
        prod = -prod;

    return prod;
}
