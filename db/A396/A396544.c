/*
 * A396544  The sum of the coreful divisors of n that are not exponential
 *          divisors of n.
 * https://oeis.org/A396544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of coreful divisors of n minus sum of exponential
 *            divisors of n * a(n) = A057723(n) - A051377(n)
 * Range:     u8 n <= 287, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* factor n */
    uint64_t t = (uint64_t)n;
    struct { uint64_t p; int e; } fac[20];
    int fc = 0;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; ++e; }
            fac[fc].p = p;
            fac[fc].e = e;
            ++fc;
        }
    }
    if (t > 1) {
        fac[fc].p = t;
        fac[fc].e = 1;
        ++fc;
    }

    /* compute products */
    seq_t prod_core = 1;
    seq_t prod_exp  = 1;

    for (int i = 0; i < fc; ++i) {
        seq_t p = (seq_t)fac[i].p;
        int e   = fac[i].e;

        /* precompute powers p^k for k = 0..e */
        seq_t pow[e + 1];
        pow[0] = 1;
        for (int k = 1; k <= e; ++k) pow[k] = pow[k - 1] * p;

        /* sum of coreful exponents: 1..e */
        seq_t sum_core = 0;
        for (int k = 1; k <= e; ++k) sum_core += pow[k];

        /* sum of exponential exponents: divisors of e */
        seq_t sum_exp = 0;
        for (int d = 1; d <= e; ++d) if (e % d == 0)
            sum_exp += pow[d];

        prod_core *= sum_core;
        prod_exp  *= sum_exp;
    }

    return prod_core - prod_exp;
}
