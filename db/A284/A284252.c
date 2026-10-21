/*
 * A284252  a(n) = smallest prime dividing n which is larger than the square
 *          of smallest prime dividing n, or 1 if no such prime exists, a(1) =
 *          1.
 * https://oeis.org/A284252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest prime divisor of n that is larger than *
 *            (the smallest prime divisor of n)^2; returns 1 if no such prime
 *            exists. * a(1) = 1
 * Range:     u8 n <= 22, u16 n <= 9920, u32 n <= 9920, u64 n <= 9920,
 *            u128 n <= 9920, big any n
 * Helpers:   factorize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factorize.h"   /* factorize(uint64_t n, uint64_t *, int *) */

seq_t a(idx_t n) {
    /* n should be at least 1.  For n = 1 by definition a(1) = 1. */
    if (n < 2)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;
    /* factorize returns primes in ascending order.  Max distinct primes
       for a 64‑bit integer is < 16, so a small static array is enough. */
    uint64_t primes[16];
    int exps[16];
    size_t cnt = factorize(m, primes, exps);
    if (cnt == 0)  /* should only happen for n == 1, handled above */
        return (seq_t)1;

    uint64_t smallest = primes[0];
    seq_t thresh = (seq_t)smallest * (seq_t)smallest;

    /* Find the first prime factor larger than thresh. */
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] > (uint64_t)thresh)
            return (seq_t)primes[i];
    }

    /* No such prime divisor exists. */
    return (seq_t)1;
}
