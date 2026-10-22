/*
 * A358593  a(n) = n! * Sum_{d|n} d^(n-d) / d!^(n/d).
 * https://oeis.org/A358593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{d|n} d^(n-d) / (d!)^(n/d)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, factor_u64, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For a given n, a(n) is the product of n! and the sum
 * over all positive divisors d of n of
 *   d^(n-d) / (d!)^(n/d)
 * The sum is always a rational number whose denominator divides n!.
 * Consequently, n! * (d^(n-d) / (d!)^(n/d)) is an integer for every d|n,
 * so the whole sum can be computed as an integer sum.
 *
 * Approach:
 *   1. Compute n!  (big integer factorial).
 *   2. Enumerate all divisors of n via its prime factorisation.
 *   3. For each divisor d:
 *        fact_d      = d!
 *        denom       = fact_d^(n/d)          (big integer power)
 *        coeff       = n! / denom            (exact integer division)
 *        num         = d^(n-d)               (big integer power)
 *        term        = coeff * num
 *        sum += term
 *   4. Return the accumulated sum.
 *
 * This keeps all intermediate values exact and uses only integer
 * arithmetic.  The divisor enumeration is O(#divisors) which is
 * small (< few thousand for n≤10000).  The algorithm runs in
 * well under the time limit for all n up to 10000. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
#include "../utils/factorial.h"
#include "../utils/big_pow.h"

/* Recursive helper to generate all divisors of n from its prime factorisation.
 * primes  - array of distinct prime factors of n
 * exps    - array of corresponding exponents
 * cnt     - number of distinct prime factors
 * idx     - current index in primes/exps
 * cur     - current partial divisor
 * divs    - output array to store divisors
 * pos     - pointer to current fill position in divs
 */
static void
gen_divs_rec(const uint64_t *primes,
             const uint64_t *exps,
             size_t cnt,
             size_t idx,
             idx_t cur,
             idx_t *divs,
             size_t *pos)
{
    if (idx == cnt) {
        divs[*pos] = cur;
        (*pos)++;
        return;
    }
    idx_t val = cur;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        gen_divs_rec(primes, exps, cnt, idx + 1, val, divs, pos);
        val *= primes[idx];
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Compute n! once */
    seq_t nf = factorial(n);

    /* Factor n into primes */
    uint64_t primes[128];
    uint64_t exps[128];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pf_cnt);

    /* Generate all divisors of n */
    /* Number of divisors of n≤10000 is less than 1300, 2000 is safe */
    idx_t divs[2000];
    size_t divcnt = 0;
    gen_divs_rec(primes, exps, pf_cnt, 0, (idx_t)1, divs, &divcnt);

    /* Sum over divisors */
    seq_t sum = 0;
    for (size_t i = 0; i < divcnt; ++i) {
        idx_t d = divs[i];

        /* factorial of the divisor */
        seq_t fact_d = factorial(d);

        /* denom = (d!)^(n/d) */
        seq_t denom = big_pow(fact_d, (seq_t)(n / d));

        /* coeff = n! / denom  (exact division) */
        seq_t coeff = nf / denom;

        /* numer = d^(n-d) */
        seq_t numer = big_pow((seq_t)d, (seq_t)(n - d));

        /* term = coeff * numer */
        seq_t term = coeff * numer;

        sum += term;
    }

    return sum;
}
