/*
 * A258650  Tenth arithmetic derivative of n.
 * https://oeis.org/A258650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415^10(n)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 191, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Compute arithmetic derivative of a 64‑bit integer */
static seq_t derivative_uint64(uint64_t n)
{
    if (n <= 1)
        return (seq_t)0;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)exps[i] * ((seq_t)n / (seq_t)primes[i]);
        res += term;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    for (int i = 0; i < 10; ++i) {
        uint64_t v = (uint64_t)cur;             /* fits uint64_t for given n range */
        cur = derivative_uint64(v);
    }
    return cur;
}
