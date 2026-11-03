/*
 * A358280  a(n) = Sum_{d|n} (d-1)!.
 * https://oeis.org/A358280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d|n} (d-1)! (OEIS A358280)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

static void add_divisor_factorials(const uint64_t *pr, const uint64_t *ex,
                                   size_t cnt, size_t pos, uint64_t current,
                                   seq_t *sum)
{
    if (pos == cnt) {
        seq_t f = factorial((idx_t)(current - 1)); /* (d-1)!  */
        *sum += f;
        return;
    }

    uint64_t val = current;
    for (uint64_t e = 0; e <= ex[pos]; ++e) {
        add_divisor_factorials(pr, ex, cnt, pos + 1, val, sum);
        if (e < ex[pos])
            val *= pr[pos];
    }
}

seq_t a(idx_t n)
{
    if (n == 0)           /* the sequence is 1‑indexed, a(1)=1  */
        return 0;

    /* factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 0;
    add_divisor_factorials(primes, exps, cnt, 0, 1, &result);
    return result;
}
