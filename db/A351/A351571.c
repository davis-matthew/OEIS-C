/*
 * A351571  Arithmetic derivative of the sum of the divisors of the largest
 *          unitary divisor of n that is an exponentially odd number.
 * https://oeis.org/A351571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, changed
 * Formula:   a(n) = arithmetic derivative of the sum of the divisors of * the
 *            largest unitary divisor of n that is an exponentially odd
 *            number. * "Exponentially odd" means that every prime exponent in
 *            its factorization * is odd. The largest such unitary divisor is
 *            the product of all prime * powers of n whose...
 * Range:     u8 n <= 16, u16 n <= 1038, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 96 terms through n = 9458 (2026-11-01)
 * Helpers:   factor_u64, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* Factor n */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* Compute d = product of p^e for odd e */
    seq_t d = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] & 1U) {                /* exponent is odd */
            seq_t p_pow = ipow_u64_idx(pr[i], (idx_t)ex[i]); /* p^e */
            d *= p_pow;
        }
    }

    /* Compute σ(d) = product over odd e of (p^(e+1) - 1)/(p - 1) */
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (ex[i] & 1U) {
            seq_t pe1   = ipow_u64_idx(pr[i], (idx_t)(ex[i] + 1));
            seq_t numer = pe1 - 1;
            seq_t denom = (seq_t)(pr[i] - 1);
            seq_t term  = numer / denom;
            sigma *= term;
        }
    }

    /* Factor sigma and apply the arithmetic derivative formula */
    uint64_t s_pr[64];
    uint64_t s_ex[64];
    size_t s_cnt = 0;
    factor_u64((uint64_t)sigma, s_pr, s_ex, &s_cnt);

    seq_t derivative = 0;
    for (size_t j = 0; j < s_cnt; ++j) {
        seq_t prime   = (seq_t)s_pr[j];
        seq_t exp     = (seq_t)s_ex[j];
        seq_t term = (sigma / prime) * exp;  /* integer division guarantees integral result */
        derivative += term;
    }

    return derivative;
}
