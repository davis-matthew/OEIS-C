/*
 * A372676  a(n) = phi(11 * n)/10.
 * https://oeis.org/A372676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = phi(11 * n) / 10 Computation: factor n, compute a(n) via
 *            multiplicativity: a(11^e) = 11^e, a(p^e) = (p-1)*p^(e-1) for p
 *            != 11. This follows from a(n) = phi(11*n)/10
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"  // provides factor_u64

seq_t a(idx_t n)
{
    if (n == 0) return 0;          // offset undefined, safe return

    /* factor_u64 writes into provided arrays; 64 slots are ample for n≤64‑bit */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        if (p == 11) {
            /* multiply by 11^e */
            seq_t t = 1;
            for (uint64_t k = 0; k < e; ++k)
                t *= (seq_t)11;
            result *= t;
        } else {
            /* multiply by (p-1)*p^(e-1) */
            seq_t t = (seq_t)(p - 1);
            for (uint64_t k = 0; k < e - 1; ++k)
                t *= (seq_t)p;
            result *= t;
        }
    }
    return result;
}
