/*
 * A277127  a(n) = n - lambda(n), where lambda(n) = A002322(n).
 * https://oeis.org/A277127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - λ(n), where λ(n) is the Carmichael function (A002322)
 * Range:     u8 n <= 271, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/pow_uint.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 0)   /* undefined for n < 1, but safe guard */
        return 0;
    uint64_t nn = (uint64_t)n;

    /* Factor n into primes; at most 64 distinct prime factors for 64‑bit n */
    size_t max_factors = 64;
    uint64_t pr[max_factors];
    uint64_t ex[max_factors];
    size_t cnt = 0;

    /* factor_u64 expects the caller to provide buffers for primes and exponents */
    factor_u64(nn, pr, ex, &cnt);

    /* lcm of the Carmichael terms for each prime power factor */
    seq_t lcm = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        seq_t term;

        if (p == 2ULL) {
            if (e == 1)
                term = 1;
            else if (e == 2)
                term = 2;
            else
                term = ((seq_t)1 << (e - 2));  /* 2^{e-2} for e >= 3 */
        } else {
            /* term = p^{e-1} * (p-1) */
            term = pow_uint(p, e - 1);
            term *= (p - 1);
        }

        seq_t g = gcd(lcm, term);
        lcm = (lcm / g) * term;
    }

    return (seq_t)nn - lcm;
}
