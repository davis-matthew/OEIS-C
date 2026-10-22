/*
 * A318491  a(n) is the numerator of Sum_{d|n} Sum_{j|d} 1/j.
 * https://oeis.org/A318491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, look
 * Formula:   a(n) = numerator of (1/n) * sum_{d|n} d * tau(d)
 * Range:     u8 n <= 23, u16 n <= 1619, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Special case: n == 0 (should not occur in OEIS) */
    if (n == 0)
        return 0;

    /* Factor n into its prime powers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute f(p^e) = sum_{k=0}^e (k+1) * p^k for each prime power,
       then multiply the results (multiplicativity). */
    seq_t S = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t fp = 0;
        seq_t pp = 1;
        for (uint64_t k = 0; k <= e; ++k) {
            fp += (k + 1) * pp;
            pp *= (seq_t)p;
        }
        S *= fp;
    }

    /* Reduce the fraction S / n to its numerator. */
    seq_t g = gcd(S, (seq_t)n);
    return S / g;
}
