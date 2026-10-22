/*
 * A359612  Largest prime factor with minimal exponent in canonical prime
 *          factorization of n.
 * https://oeis.org/A359612
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime p such that the exponent of p in the
 *            canonical prime factorization of n is the minimal exponent among
 *            all primes in that factorization
 * Range:     u8 n <= 256, u16 n <= 9650, u32 n <= 9650, u64 n <= 9650,
 *            u128 n <= 9650, big any n
 * Helpers:   factorize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factorize.h"

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;  /* offset is 2, return 0 for n<2 */

    /* arrays to hold up to 128 prime factors (enough for uint64_t) */
    uint64_t primes[128];
    int exps[128];
    size_t cnt = factorize((uint64_t)n, primes, exps);
    if (cnt == 0) return (seq_t)0; /* should not happen for n>=2 */

    int min_exp = exps[0];
    for (size_t i = 1; i < cnt; ++i) {
        if (exps[i] < min_exp) min_exp = exps[i];
    }

    uint64_t max_prime = primes[0];
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] == min_exp && primes[i] > max_prime) {
            max_prime = primes[i];
        }
    }
    return (seq_t)max_prime;
}
