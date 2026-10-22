/*
 * A355003  a(n) = gcd(A003415(n), A277791(n)), where A003415 is the
 *          arithmetic derivative and A277791 is the denominator of sum of
 *          reciprocals of proper divisors of n.
 * https://oeis.org/A355003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A003415(n), A277791(n))
 * Range:     u8 n <= 332, u16 n <= 9601, u32 n <= 9601, u64 n <= 9601,
 *            u128 n <= 9601, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;  /* derivative 0, denom 1 → gcd 1 */

    uint64_t n64 = (uint64_t)n;
    seq_t sn = (seq_t)n64;

    /* factorization */
    const size_t MAX_FACTORS = 32;
    uint64_t pr[MAX_FACTORS];
    uint64_t ex[MAX_FACTORS];
    size_t cnt = 0;
    factor_u64(n64, pr, ex, &cnt);

    /* arithmetic derivative d(n) = n * Σ e/p  */
    seq_t derivative = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        seq_t e = (seq_t)ex[i];
        /* n/p is integer because p | n */
        derivative += e * (sn / p);
    }

    /* sigma(n) = Π (p^(e+1)-1)/(p-1) ; compute multiplicatively */
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        seq_t term = 1;
        seq_t t = 1;
        for (uint64_t k = 0; k < e; ++k) {
            t *= (seq_t)p;
            term += t;
        }
        sigma *= term;
    }

    /* A277791(n) = n / gcd(n, sigma-1) */
    seq_t temp = sigma - (seq_t)1;
    seq_t g = gcd(sn, temp);
    seq_t denom = sn / g;

    /* final a(n) = gcd(derivative, denom) */
    return gcd(derivative, denom);
}
