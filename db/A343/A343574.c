/*
 * A343574  a(n) = Sum_{d|n} d^d * binomial(d+n/d-1, d).
 * https://oeis.org/A343574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^d * binomial(d + n/d - 1, d)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

static void _sum_divisors(idx_t pi, idx_t curr, const uint64_t *pr, const uint64_t *ex,
                          size_t cnt, seq_t *sum, idx_t n)
{
    if (pi == cnt) {
        idx_t d = curr;
        seq_t dseq = (seq_t)d;
        seq_t d_pow = big_pow(dseq, dseq);
        idx_t n_div_d = n / d;
        idx_t val = d + n_div_d - 1;
        seq_t bin = binomial(val, d);
        *sum += d_pow * bin;
        return;
    }
    uint64_t p = pr[pi];
    size_t e = ex[pi];
    idx_t val = 1;
    for (size_t k = 0; k <= e; ++k) {
        _sum_divisors(pi + 1, curr * val, pr, ex, cnt, sum, n);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    seq_t sum = 0;
    _sum_divisors(0, 1, primes, exps, cnt, &sum, n);
    return sum;
}
