/*
 * A254503  Möbius transform of A034448.
 * https://oeis.org/A254503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = ∏_{p^e‖n} ( p if e = 1 * p^{e-1}(p-1) if e > 1 ) ;
 *            multiplicative * (equivalently a(n) = sum_{d^2|n} μ(d)·n/d)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (e == 1) {
            result *= (seq_t)p;
        } else {
            seq_t term = (seq_t)1;
            /* compute p^(e-1) */
            for (uint64_t j = 0; j < e - 1; ++j)
                term *= (seq_t)p;
            term *= ((seq_t)p - (seq_t)1);
            result *= term;
        }
    }

    return result;
}
