/*
 * A344777  a(n) = Sum_{d|n} (-1)^(n/d-1) * binomial(d+n/d-1, d).
 * https://oeis.org/A344777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} (-1)^(n/d-1) * binomial(d+n/d-1, d)
 * Range:     i8 n <= 17, i16 n <= 79, i32 n <= 287, i64 n <= 1119,
 *            i128 n <= 4306, big any n
 * Helpers:   binom, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/factor_u64.h"  /* factor_u64 */
#include "../utils/binom.h"      /* binom */

static seq_t rec_div(const uint64_t *pr, const uint64_t *ex, size_t pos,
                     size_t cnt, unsigned long long cur, uint64_t N)
{
    if (pos == cnt) {
        uint64_t d  = cur;
        uint64_t nd = N / d;          /* n/d, integer because d|n */
        seq_t   bin = binom(d + nd - 1, d);
        /* sign = (-1)^(nd-1) */
        if ((nd - 1) & 1ULL)
            return -bin;
        else
            return bin;
    }

    seq_t sum = 0;
    unsigned long long pow = 1;
    for (size_t e = 0; e <= ex[pos]; ++e) {
        sum += rec_div(pr, ex, pos + 1, cnt, cur * pow, N);
        pow *= pr[pos];
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    uint64_t N = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(N, primes, exps, &cnt);
    return rec_div(primes, exps, 0, cnt, 1ULL, N);
}
