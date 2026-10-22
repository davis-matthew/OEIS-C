/*
 * A359204  a(n) = Sum_{d|n} (n/d) * 4^(n-d).
 * https://oeis.org/A359204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} (n/d) * 4^(n-d)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

/* Recursive enumerator of all divisors of n.
   pr[pos..] – array of prime factors,
   exp[pos..] – their exponents,
   pcnt – number of remaining primes,
   cur – current divisor being built,
   acc – accumulator for the sum. */
static void dfs_divisors(idx_t n,
                         const uint64_t *pr,
                         const uint64_t *exp,
                         size_t pcnt,
                         idx_t cur,
                         seq_t *acc)
{
    if (pcnt == 0) {
        /* cur is a divisor of n */
        seq_t term = big_pow((seq_t)4, (seq_t)(n - cur));
        term *= (seq_t)(n / cur);
        *acc += term;
        return;
    }

    uint64_t p   = pr[0];
    uint64_t e   = exp[0];
    idx_t     cur_val = cur;
    for (size_t i = 0; i <= e; ++i) {
        dfs_divisors(n, pr + 1, exp + 1, pcnt - 1, cur_val, acc);
        cur_val *= p;
    }
}

seq_t a(idx_t n)
{
    /* Prime factorisation of n.  The helper factor_u64 expects a
       uint64_t argument, which is compatible with idx_t (uint64_t) */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   pcnt;

    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t sum = 0;
    dfs_divisors(n, primes, exps, pcnt, (idx_t)1, &sum);
    return sum;
}
