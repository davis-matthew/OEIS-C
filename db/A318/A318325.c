/*
 * A318325  a(n) = Sum_{d|n} [moebius(n/d) > 0]*(sigma(d)-d).
 * https://oeis.org/A318325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [mu(n/d) > 0]*(sigma(d)-d)
 * Range:     u8 n <= 16, u16 n <= 4225, u32 n <= 8558, u64 n <= 8558,
 *            u128 n <= 8558, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function enumerates all squarefree divisors k of n with an even
   number of prime factors, then adds sigma(n/k)-n/k to the total. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* utility functions available from the build */
#include "../utils/factor_u64.h"   /* factor_u64(n, primes, exps, &cnt) */
#include "../utils/pow_seq.h"      /* pow_seq(base, exp) */

/* ---------------------------------------------------------------- */
/* Computes a(n) for the OEIS sequence A318325                      */
/* ---------------------------------------------------------------- */
seq_t a(idx_t n) {
    /* Factor n: primes[i] ^ exps[i]   (i = 0 .. cnt-1) */
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;

    /* Enumerate all squarefree divisors k of n, i.e. subsets of primes
       with an even number of selected primes. */
    uint64_t total_masks = 1ULL << cnt;
    for (uint64_t mask = 0; mask < total_masks; ++mask) {
        /* mu(k) > 0  <=>  k is squarefree with an even number of primes */
        if (__builtin_popcountll(mask) & 1U)   /* odd number of primes -> skip */
            continue;

        seq_t part  = 1;   /* product of sigma(p^exp) for d */
        seq_t dval  = 1;   /* product of p^exp for d = n/k */

        for (size_t i = 0; i < cnt; ++i) {
            /* Determine whether prime p[i] is in k (i.e. mask bit set) */
            int bit = (mask >> i) & 1U;
            seq_t p   = (seq_t)primes[i];
            int exp   = (int)(exps[i] - bit); /* exponent in d = n/k */

            /* sigma(p^exp) = (p^(exp+1) - 1)/(p - 1) */
            seq_t sigma = (pow_seq(p, exp + 1) - 1) / (p - 1);
            part *= sigma;

            /* compute p^exp for dval   (pow_seq handles exp == 0 correctly) */
            seq_t pow_part = pow_seq(p, exp);
            dval *= pow_part;
        }

        sum += part - dval;   /* sigma(d) - d */
    }
    return sum;
}
