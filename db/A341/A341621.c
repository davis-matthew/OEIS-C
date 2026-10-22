/*
 * A341621  a(n) is the exponent of the least power of 2 that when multiplied
 *          by n makes the product abundant, or -1 if n itself is a power of
 *          2.
 * https://oeis.org/A341621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the minimal k such that 2^k * n is abundant, or -1 if n
 *            is a power of two. Abundant means σ(m) > 2m
 * Range:     i8 n <= 12, i16 n <= 486, i32 n <= 8620, i64 n <= 8620,
 *            i128 n <= 8620, big any n
 * Helpers:   factor_u64, is_power_of_two, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm:
   1. If n is a power of two, return -1.
   2. Compute the exponent a of 2 in n and the odd part M.
   3. For the odd part, compute M = ∏ p^e  and
      O = ∏ σ(p^e) = ∏ (p^(e+1)-1)/(p-1).
   4. Let diff = O - M.  (diff ≥ 1 for all odd n ≠ 1).
   5. Find the smallest integer t ≥ a+1 such that
          2^t * diff > O
      The corresponding exponent is k = t - a - 1.
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_power_of_two.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t xn = (seq_t)n;

    /* Case n is a power of two: no abundant multiple exists. */
    if (is_power_of_two(xn))
        return (seq_t)-1;

    /* Count the exponent a of 2 in n. */
    idx_t a2 = 0;
    uint64_t tmp = (uint64_t)n;
    while ((tmp & 1ULL) == 0) {
        ++a2;
        tmp >>= 1;
    }

    /* Factorize n and separate odd primes. */
    size_t cnt = 0;
    uint64_t pr[128], ex[128];
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t M = 1;   /* odd part product */
    seq_t O = 1;   /* σ of odd part */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        if (p == 2ULL)
            continue; /* skip the power of two part */

        /* p^e */
        seq_t pow_pe = seq_pow((seq_t)p, (int)e);
        /* p^(e+1) */
        seq_t pow_pe1 = seq_pow((seq_t)p, (int)(e + 1));

        M *= pow_pe;

        seq_t num   = pow_pe1 - 1;                 /* numerator of σ(p^e) */
        seq_t denom = (seq_t)(p - 1);              /* denominator */
        seq_t term  = num / denom;                 /* exact division */
        O *= term;
    }

    /* Difference between σ(odd) and odd part.  diff ≥ 1. */
    seq_t diff = O - M;

    /* Find the least t ≥ a2+1 such that 2^t * diff > O. */
    for (idx_t t = a2 + 1; ; ++t) {
        seq_t pow2 = ((seq_t)1) << t;   /* 2^t  (t is small, ≤ 64) */
        seq_t prod = pow2 * diff;
        if (prod > O) {
            seq_t k = (seq_t)(t - a2 - 1);
            return k;
        }
    }
}
