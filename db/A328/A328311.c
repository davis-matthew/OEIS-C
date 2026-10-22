/*
 * A328311  a(n) = 1 + A051903(A003415(n)) - A051903(n), a(1) = 0 by
 *          convention.
 * https://oeis.org/A328311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + A051903(A003415(n)) - A051903(n)
 * Range:     u8 n <= 620, u16 n <= 9204, u32 n <= 9204, u64 n <= 9204,
 *            u128 n <= 9204, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 1) return 0;

    /* Factor n */
    uint64_t pr[64], ex[64];
    size_t n_cnt = 0;
    factor_u64(n, pr, ex, &n_cnt);

    /* Maximum exponent in factorization of n */
    seq_t maxexp_n = 0;
    for (size_t i = 0; i < n_cnt; ++i) {
        seq_t e = (seq_t)ex[i];
        if (e > maxexp_n) maxexp_n = e;
    }

    /* Compute arithmetic derivative A003415(n) = sum e_i * (n / p_i) */
    seq_t derivative = 0;
    for (size_t i = 0; i < n_cnt; ++i) {
        derivative += (seq_t)ex[i] * (seq_t)(n / pr[i]);
    }

    /* Factor the derivative (assumes it fits in 64 bits for n <= 10000) */
    uint64_t deriv_u64 = (uint64_t)derivative;
    uint64_t pr2[64], ex2[64];
    size_t d_cnt = 0;
    factor_u64(deriv_u64, pr2, ex2, &d_cnt);

    /* Maximum exponent in factorization of derivative */
    seq_t maxexp_deriv = 0;
    for (size_t i = 0; i < d_cnt; ++i) {
        seq_t e = (seq_t)ex2[i];
        if (e > maxexp_deriv) maxexp_deriv = e;
    }

    return 1 + maxexp_deriv - maxexp_n;
}
