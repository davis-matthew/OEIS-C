/*
 * A245785  Denominator of (n/tau(n) + sigma(n)/n).
 * https://oeis.org/A245785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of (n/tau(n) + sigma(n)/n)
 * Range:     u8 n <= 11, u16 n <= 1861, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, factor_u64, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"

/*
 * Computes the denominator of n/tau(n) + sigma(n)/n in reduced form.
 * Uses the definition:
 *   (n^2 + sigma(n)*tau(n)) / (n * tau(n))
 * and reduces the fraction by the greatest common divisor.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined; return 0 for safety */

    /* factor n */
    size_t cnt = 0;
    uint64_t pr[64], ex[64];
    factor_u64((uint64_t)n, pr, ex, &cnt);

    /* compute tau and sigma */
    seq_t tau = 1;
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        unsigned e = (unsigned)ex[i];

        tau *= (seq_t)(e + 1);

        /* p^(e+1) */
        seq_t pe = 1;
        for (unsigned j = 0; j < e + 1; ++j) pe *= p;
        seq_t factor_sigma = (pe - 1) / (p - 1);
        sigma *= factor_sigma;
    }

    seq_t nn = (seq_t)n;
    seq_t numerator   = nn * nn + sigma * tau;
    seq_t denom_raw   = nn * tau;
    seq_t g = gcd(numerator, denom_raw);
    return denom_raw / g;
}
