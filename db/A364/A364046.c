/*
 * A364046  Expansion of Sum_{k>0} x^(4*k) / (1 + x^(5*k)).
 * https://oeis.org/A364046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, d==4 (mod 5)} (-1)^d
 * Range:     i8 n <= 99, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 99 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Recursively accumulate contributions of every divisor d of n
 * (represented by cur).  For each divisor that satisfies d % 5 == 4
 * we add (+1) if d is even, otherwise (-1).  The recursion
 * enumerates all divisors exactly once. */
static seq_t _sum_divs(const uint64_t *primes, const uint64_t *exps,
                       size_t cnt, size_t idx, uint64_t cur)
{
    if (idx == cnt) {
        if ((cur % 5ULL) == 4ULL)        /* d % 5 == 4  */
            return (cur & 1ULL) ? -1 : +1; /* (-1)^d */
        else
            return 0;
    }

    seq_t sum = 0;
    uint64_t mult = 1ULL;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        uint64_t new_cur = cur * mult;
        sum += _sum_divs(primes, exps, cnt, idx + 1, new_cur);
        mult *= primes[idx];
    }
    return sum;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset starts at 1; behaviour for 0 not required */
        return 0;

    /* Factor n */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the sum over divisors */
    return _sum_divs(primes, exps, cnt, 0, 1ULL);
}
