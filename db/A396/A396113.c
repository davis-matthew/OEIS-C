/*
 * A396113  a(n) = sopf(n) * phi(n), where sopf = A008472 and phi = A000010.
 * https://oeis.org/A396113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sopf(n) * phi(n), where sopf(n) = sum of distinct prime
 *            factors of n
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorize, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/factorize.h"

static seq_t sopf(idx_t n)
{
    /* Sum of distinct prime factors of n (A008472). */
    /* n is treated as uint64_t; at most 12 distinct primes fit into that range. */
    uint64_t primes[12];
    int exps[12];        /* array for exponents, not used */
    size_t cnt = factorize((uint64_t)n, primes, exps);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum += (seq_t)primes[i];
    return sum;
}

seq_t a(idx_t n)
{
    seq_t sum_primes = sopf(n);
    seq_t phi_val    = phi(n);
    return sum_primes * phi_val;
}
