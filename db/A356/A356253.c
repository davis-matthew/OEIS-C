/*
 * A356253  a(n) is the largest coefficient of P(x) := Product_{k} (x + p_k),
 *          where (p_k) are the primes dividing n listed with multiplicity.
 * https://oeis.org/A356253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 4095, u32 n <= 9964, u64 n <= 9964,
 *            u128 n <= 9964, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A356253: a(n) is the largest coefficient of
   P(x) = ∏_k (x + p_k), where (p_k) are the primes dividing n with multiplicity.
   Implementation: factor n, build the list of prime factors with multiplicity,
   then compute all elementary symmetric sums by dynamic programming and
   return the maximum.
*/
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* Factor n: primes in pr[], exponents in ex[], count in cnt */
    uint64_t pr[64], ex[64];
    size_t cnt, i;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Total multiplicity m */
    size_t m = 0;
    for (i = 0; i < cnt; ++i) m += ex[i];
    if (m == 0) return (seq_t)1;           /* n==1 case handled above */

    /* Compute list of primes with multiplicity */
    uint64_t fact_list[64];
    size_t pos = 0;
    for (i = 0; i < cnt; ++i) {
        size_t e = ex[i];
        uint64_t p = pr[i];
        while (e--) fact_list[pos++] = p;
    }

    /* DP for elementary symmetric sums */
    seq_t coeff[65] = {0};
    coeff[0] = (seq_t)1;
    size_t processed = 0;

    for (i = 0; i < m; ++i) {
        seq_t p_val = (seq_t)fact_list[i];
        /* update from high to low */
        size_t j;
        for (j = processed + 1; j > 0; --j) {
            coeff[j] += coeff[j - 1] * p_val;
        }
        ++processed;
    }

    /* Find maximum coefficient */
    seq_t max_val = coeff[0];
    for (i = 1; i <= m; ++i) {
        if (coeff[i] > max_val) max_val = coeff[i];
    }
    return max_val;
}
