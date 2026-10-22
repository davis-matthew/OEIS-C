/*
 * A374903  Denominator of the mean number of divisors of the divisors of n.
 * https://oeis.org/A374903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = τ(n) / gcd( τ(n), Σ_{d|n} τ(d) ) where τ(k) is the number
 *            of divisors of k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _sum_tau_divisors, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/_sum_tau_divisors.h"

seq_t a(idx_t n)
{
    /* Handle n = 0 or negative indices with undefined behavior; the
       problem guarantees n >= 1. */
    /* Factor n into its prime powers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute τ(n) = Π (exps[i] + 1). */
    seq_t tau_n = 1;
    for (size_t i = 0; i < cnt; ++i) {
        tau_n *= (seq_t)(exps[i] + 1);
    }

    /* Compute Σ τ(d) over all divisors d of n. */
    seq_t sum_tau = 0;
    _sum_tau_divisors(primes, exps, cnt, 0, (seq_t)1, &sum_tau);

    /* Reduce the fraction and return the denominator. */
    seq_t g = gcd(sum_tau, tau_n);
    return tau_n / g;
}
