/*
 * A361685  Number of iterations of sopf until reaching a prime.
 * https://oeis.org/A361685
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{m : sopf^m(n) is prime} * where sopf(n) is the sum of
 *            the distinct prime factors of n
 * Range:     u8 n <= 2366, u16 n <= 9544, u32 n <= 9544, u64 n <= 9544,
 *            u128 n <= 9544, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/is_prime.h"     /* is_prime(idx_t n) */

static seq_t sopf_uint64(uint64_t n)
{
    /* Sum distinct prime factors of n.  n > 1. */
    uint64_t primes[64];
    uint64_t exps[64];   /* exponents are ignored but required by factor_u64 */
    size_t   cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum += (seq_t)primes[i];   /* only distinct primes */
    return sum;
}

seq_t a(idx_t n)
{
    /* a(2) is the first term.  For n < 2 behavior is unspecified. */
    if (n < 2)
        return 0;   /* placeholder – never called by OEIS tester */

    if (is_prime(n))
        return 0;

    seq_t iterations = 0;
    uint64_t current = (uint64_t)n;

    while (!is_prime((idx_t)current)) {
        current = (uint64_t)sopf_uint64(current);
        ++iterations;
    }

    return iterations;
}
