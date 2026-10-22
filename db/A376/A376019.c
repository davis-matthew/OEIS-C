/*
 * A376019  a(n) = Sum_{d|n} d^n * binomial(n/d-1,d-1).
 * https://oeis.org/A376019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^n * binomial(n/d-1,d-1)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 15, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binomial, factor_u64, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/binomial.h"
#include "../utils/pow_int.h"

/* Recursively enumerate all divisors of n and accumulate the sum. */
static void rec_divs(idx_t i, idx_t nf,
                     const uint64_t p[], const uint64_t e[],
                     idx_t cur_d, idx_t n, seq_t *sum)
{
    if (i == nf) {
        seq_t pow_val = pow_int((seq_t)cur_d, n);
        idx_t q = n / cur_d;
        idx_t binom_a = q - 1;
        idx_t binom_b = cur_d - 1;
        seq_t binom_val = binomial(binom_a, binom_b);
        *sum += pow_val * binom_val;
        return;
    }
    idx_t d = cur_d;
    for (uint64_t exp = 0; exp <= e[i]; ++exp) {
        rec_divs(i + 1, nf, p, e, d, n, sum);
        d *= (idx_t)p[i];
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* outside the sequence, safe fallback */

    uint64_t factors[64];
    uint64_t exponents[64];
    size_t nf = 0;
    factor_u64(n, factors, exponents, &nf);

    seq_t sum = (seq_t)0;
    rec_divs(0, nf, factors, exponents, (idx_t)1, n, &sum);
    return sum;
}
