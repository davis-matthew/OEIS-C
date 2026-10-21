/*
 * A254340  Sum of the distinct prime factors of n plus n+1: a(n) = A008472(n)
 *          + n + 1.
 * https://oeis.org/A254340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of distinct prime factors of n + n + 1
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence definition uses the sum of the distinct prime factors of n,
 * sometimes called the radical of n.  This implementation obtains the
 * distinct prime factors by factoring n with the helper
 * `factor_u64` and then adds the result to n+1.  The helper returns
 * each distinct prime factor in the array `pr`, so a simple sum over
 * that array gives the desired value. */

#include <stdint.h>
#include "../utils/factor_u64.h"

/* Sum of the distinct prime factors of n.
 * Returns 0 for n < 2. */
static seq_t distinct_prime_sum(uint64_t n) {
    if (n < 2) return 0;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    /* factor_u64 fills primes, exps and cnt */
    factor_u64(n, primes, exps, &cnt);
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i)
        sum += (seq_t)primes[i];
    return sum;
}

/* Sequence A254340: a(n) = sum of distinct primes dividing n + n + 1. */
seq_t a(idx_t n) {
    seq_t sum = distinct_prime_sum((uint64_t)n);
    return sum + (seq_t)n + 1;
}
