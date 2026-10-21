/*
 * A244974  Sum of numbers m <= n whose set of prime divisors is a subset of
 *          the set of prime divisors of n.
 * https://oeis.org/A244974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=1..n, set of prime divisors of m ⊆ set of prime
 *            divisors of n} m
 * Range:     u8 n <= 41, u16 n <= 1469, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _factor_distinct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/_factor_distinct.h"

/* Find the distinct prime factors of n and store them in `primes`.  */

/* Recursively generate all products of the given primes (in reverse order), */
/* with product <= limit, and add each product to *sum.  The current product  */
/* is held in `cur`.  The recursion stops when idx == 0, where we add cur      */
static void _gen(const uint64_t *primes, size_t idx, idx_t limit,
                 uint64_t cur, seq_t *sum)
{
    if (idx == 0) {
        *sum += (seq_t)cur;
        return;
    }
    uint64_t p = primes[idx - 1];
    uint64_t prod = cur;
    while (prod <= limit) {
        _gen(primes, idx - 1, limit, prod, sum);
        if (prod > limit / p) break;
        prod *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* The maximum number of distinct prime factors for n ≤ 2⁶⁴ is < 64. */
    uint64_t primes[64];
    size_t cnt;
    _factor_distinct(n, primes, &cnt);

    seq_t sum = 0;
    _gen(primes, cnt, n, 1, &sum);
    return sum;
}
