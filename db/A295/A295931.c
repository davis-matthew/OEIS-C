/*
 * A295931  Number of ways to write n in the form n = (x^y)^z where x, y, and
 *          z are positive integers.
 * https://oeis.org/A295931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k | gcd of prime exponents of n} tau(k)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, tau_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where tau(k) is the number of divisors of k.  For n = 1, a(1) = 1. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"   /* for factor_u64 */
#include "../utils/tau_small.h"

static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Factorize n using the provided helper */
    uint64_t primes[64], exps[64];
    size_t prime_count = 0;
    factor_u64((uint64_t)n, primes, exps, &prime_count);

    /* Compute gcd of exponents */
    idx_t g = exps[0];
    for (size_t i = 1; i < prime_count; ++i)
        g = gcd_idx(g, exps[i]);

    /* Enumerate divisors of g and accumulate tau(divisor) */
    seq_t sum = 0;
    /* simple enumeration of divisors since g <= 64 (for n <= 1e4) */
    for (idx_t d = 1; d <= g; ++d) {
        if (g % d == 0) {
            sum += tau_small(d);
        }
    }
    return sum;
}
