/*
 * A308135  Sum of non-coreful divisors of n.
 * https://oeis.org/A308135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = σ(n) - sum of coreful divisors of n
 * Range:     u8 n <= 22, u16 n <= 3543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9599 (2026-11-01)
 * Helpers:   factor_u64, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* For n = 1 the only divisor is 1, which is both coreful and a
       divisor, so a(1) = 0. */
    if (n == 1)
        return 0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;

    /* Factorise n into prime factors and their exponents. */
    factor_u64(n, primes, exps, &cnt);

    seq_t sigma   = 1;   /* σ(n) – sum of all divisors */
    seq_t coreful = 1;   /* sum of coreful divisors */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        idx_t    e = exps[i];        /* exponent of prime p in n */

        /* p^(e+1) */
        seq_t pow_e1 = ipow_u64_idx(p, e + 1);

        /* sum_{k=0..e} p^k = (p^(e+1)-1)/(p-1) */
        seq_t sum_all = (pow_e1 - 1) / (p - 1);
        sigma *= sum_all;

        /* sum_{k=1..e} p^k = (p^(e+1)-p)/(p-1) */
        seq_t sum_core = (pow_e1 - p) / (p - 1);
        coreful *= sum_core;
    }

    return sigma - coreful;
}
