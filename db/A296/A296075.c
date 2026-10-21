/*
 * A296075  Sum of deficiencies of divisors of n.
 * https://oeis.org/A296075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{d|n} (2*d - sigma(d))
 * Range:     i8 n <= 12, i16 n <= 1030, i32 n <= 8931, i64 n <= 8931,
 *            i128 n <= 8931, big any n
 * Helpers:   factor_u64, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (valid C11 and C++17) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"
#include "../utils/seq_mul.h"

/* Recursive helper that enumerates all divisors of n from its prime
   factorisation and accumulates the sum of deficiencies. */
static void
sum_deficiencies_rec(const uint64_t *primes,
                     const uint64_t *exps,
                     size_t idx,
                     size_t total,
                     seq_t cur_d,
                     seq_t cur_sigma,
                     seq_t *sum)
{
    if (idx == total) {
        seq_t two_d = seq_mul(cur_d, (seq_t)2);
        seq_t deficiency = two_d - cur_sigma;
        *sum += deficiency;
        return;
    }

    uint64_t p   = primes[idx];
    uint64_t emax = exps[idx];

    for (uint64_t e = 0; e <= emax; ++e) {
        seq_t d_part = pow_uint(p, e);
        seq_t new_d  = seq_mul(cur_d, d_part);

        seq_t term = (pow_uint(p, e + 1) - 1) / ((seq_t)p - 1);
        seq_t new_sigma = seq_mul(cur_sigma, term);

        sum_deficiencies_rec(primes, exps, idx + 1, total,
                             new_d, new_sigma, sum);
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but guard */

    uint64_t nn   = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 0;
    sum_deficiencies_rec(primes, exps, 0, cnt,
                         (seq_t)1, (seq_t)1, &result);
    return result;
}
