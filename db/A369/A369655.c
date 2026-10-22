/*
 * A369655  a(n) = 1 if n' is odd and also n'' is odd, otherwise 0, where n'
 *          stands for the arithmetic derivative, A003415.
 * https://oeis.org/A369655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the arithmetic derivative n' is odd and the
 *            derivative of n', i.e. n'', is also odd; otherwise 0
 * Range:     u8 n <= 9421, u16 n <= 9421, u32 n <= 9421, u64 n <= 9421,
 *            u128 n <= 9421, big any n
 * Helpers:   derivative_parity, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/derivative_parity.h"

/* Main sequence function as required by the build system. */
seq_t a(idx_t n)
{
    /* For n = 0 or 1 the arithmetic derivative is 0 (even). */
    if (n <= 1U) {
        return (seq_t)0;
    }

    /* 1. Factorise n.  Most build configurations use 64‑bit values. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt_n = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt_n);

    /* 2. Determine parity of n'. */
    bool odd_nprime = derivative_parity((uint64_t)n, primes, exps, cnt_n);

    /* 3. Compute n' as a 64‑bit value (always sufficient for n ≤ 10 000). */
    uint64_t d_val = 0;
    for (size_t i = 0; i < cnt_n; ++i) {
        d_val += exps[i] * (n / primes[i]);
    }

    /* 4. Determine parity of n''. */
    bool odd_dprime = false;
    if (d_val != 0) {
        uint64_t primes_d[64];
        uint64_t exps_d[64];
        size_t cnt_d = 0;
        factor_u64(d_val, primes_d, exps_d, &cnt_d);
        odd_dprime = derivative_parity(d_val, primes_d, exps_d, cnt_d);
    }

    /* 5. Result is 1 only if both parities are odd. */
    return (seq_t)(odd_nprime && odd_dprime ? 1 : 0);
}
