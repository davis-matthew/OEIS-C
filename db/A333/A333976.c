/*
 * A333976  Number of pairs of divisors of n, (d1,d2), such that d1 <= d2 and
 *          gcd(d1,d2) > 1.
 * https://oeis.org/A333976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (tau(n)^2 - tau(n^2) + tau(n) - 1) / 2
 * Range:     u8 n <= 719, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes number of pairs of divisors (d1,d2) of n with d1 <= d2 and gcd(d1,d2) > 1. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* factor n into primes */
    uint64_t nn = (uint64_t)n;
    uint64_t pr[20];
    uint64_t ex[20];
    size_t cnt = 0;
    factor_u64(nn, pr, ex, &cnt);

    /* compute tau(n) and tau(n^2) */
    seq_t tau = 1;
    seq_t tau2 = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)ex[i];
        tau  *= (e + 1);
        tau2 *= (2 * e + 1);
    }

    seq_t t = tau * tau - tau2 + tau - 1;
    return t / 2;
}
