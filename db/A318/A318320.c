/*
 * A318320  a(n) = (psi(n) - phi(n))/2.
 * https://oeis.org/A318320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = (psi(n) - phi(n))/2
 * Range:     u8 n <= 209, u16 n <= 9903, u32 n <= 9903, u64 n <= 9903,
 *            u128 n <= 9903, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res = res * base;
        base = base * base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t phi_prod = 1;
    seq_t psi_prod = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = (idx_t)exps[i];
        seq_t p_pow = pow_seq(p, e);
        seq_t p_pow_div = p_pow / p;

        seq_t phi_factor = p_pow - p_pow_div;
        seq_t psi_factor = p_pow + p_pow_div;

        phi_prod = phi_prod * phi_factor;
        psi_prod = psi_prod * psi_factor;
    }

    return (psi_prod - phi_prod) / 2;
}
