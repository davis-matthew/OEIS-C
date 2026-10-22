/*
 * A396336  Dirichlet convolution of A047994 with A023900.
 * https://oeis.org/A396336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n, e≥2} (e-1)(p-1) , a(n)=0 if any exponent =1;
 *            a(1)=1
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"  /* assumed to provide factor_u64 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    /* Factorize n into primes and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        if (e < 2) return (seq_t)0;          /* non‑powerful number */
        seq_t term = ((seq_t)(e - 1)) * ((seq_t)(primes[i] - 1));
        result *= term;
    }
    return result;
}
