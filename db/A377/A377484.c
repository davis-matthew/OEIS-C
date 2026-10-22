/*
 * A377484  a(n) = Product_{d|n, d>1} (d - 1).
 * https://oeis.org/A377484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{d>1, d|n} (d-1) (OEIS A377484)
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 59, u64 n <= 239,
 *            u128 n <= 719, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Recursively visit all divisors of n.  'cur' is the divisor built so far
   and 'result' accumulates the product of (d-1).  The divisor 1 is
   ignored, as required by the definition. */
static inline void
_gen_divs(size_t idx, size_t cnt,
          const uint64_t *pr, const uint64_t *ex,
          seq_t cur, seq_t *result)
{
    if (idx == cnt) {
        if (cur > (seq_t)1) {
            *result *= (cur - (seq_t)1);
        }
        return;
    }

    seq_t cur_mul = cur;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        _gen_divs(idx + 1, cnt, pr, ex, cur_mul, result);
        cur_mul *= (seq_t)pr[idx];
    }
}

seq_t a(idx_t n)
{
    /* OEIS offset is 1; a(1) = 1 by definition. */
    if (n <= (idx_t)1) {
        return (seq_t)1;
    }

    /* Factor n.  primes[] holds the distinct prime factors, ex[] their
       exponents, and cnt the number of factors. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod = (seq_t)1;
    _gen_divs(0, cnt, primes, exps, (seq_t)1, &prod);
    return prod;
}
