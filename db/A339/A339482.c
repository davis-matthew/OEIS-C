/*
 * A339482  a(n) = Sum_{d|n} d^(n-d+1) * binomial(d+n/d-2, d-1).
 * https://oeis.org/A339482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(n-d+1) * binomial(d + n/d - 2, d-1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, binomial, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset 1: a(1) is first term. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* provides factor_u64() */
#include "../utils/binomial.h"    /* provides binomial() */
#include "../utils/big_pow.h"     /* provides big_pow() */

/* Recursive helper: accumulate contribution of all divisors */
static void sum_divisors(idx_t pos, idx_t current, idx_t n,
                         seq_t *sum,
                         const uint64_t *primes,
                         const uint64_t *exps,
                         size_t factor_cnt)
{
    if (pos == factor_cnt) {
        /* current is a divisor d of n */
        seq_t pow_term   = big_pow((seq_t)current,
                                   (seq_t)(n - current + 1));
        idx_t n_div_d    = n / current;
        seq_t bin_term   = binomial(current + n_div_d - 2,
                                    current - 1);
        *sum += pow_term * bin_term;
        return;
    }

    idx_t p  = (idx_t)primes[pos];
    idx_t val = current;
    for (int e = 0; e <= (int)exps[pos]; ++e) {
        sum_divisors(pos + 1, val, n, sum, primes, exps, factor_cnt);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety, not used per offset   */

    /* Prime factorisation of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t factor_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &factor_cnt);

    seq_t result = 0;
    sum_divisors(0, (idx_t)1, n, &result, primes, exps, factor_cnt);
    return result;
}
