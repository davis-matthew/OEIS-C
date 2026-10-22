/*
 * A383010  a(n) = -Sum_{d|n} (-n)^d.
 * https://oeis.org/A383010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -Sum_{d|n} (-n)^d
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 19,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

/* Recursive helper to enumerate divisors of n. */
static void _divisor_recurse(idx_t idx, idx_t cur, idx_t n,
                             const uint64_t *pr, const uint64_t *ex,
                             size_t cnt, seq_t *sum)
{
    if (idx == cnt) {
        seq_t term = big_pow((seq_t)n, (seq_t)cur);
        if (cur & 1) term = -term;   /* (-n)^d = (-1)^d * n^d */
        *sum += term;
        return;
    }

    idx_t p = pr[idx];
    idx_t val = cur;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        _divisor_recurse(idx + 1, val, n, pr, ex, cnt, sum);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt;

    /* Factor n into primes and exponents. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    _divisor_recurse(0, 1, n, primes, exps, cnt, &sum);
    return -sum;
}
