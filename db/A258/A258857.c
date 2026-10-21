/*
 * A258857  Seventh pi-based arithmetic derivative of n.
 * https://oeis.org/A258857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A258851^7(n) (7‑fold application of the π‑based
 *            arithmetic derivative)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 4095, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/compute_pi.h"

/* Compute the π‑based arithmetic derivative of a single integer. */
static seq_t derivative(seq_t n)
{
    if ((uint64_t)n < 2) {
        return (seq_t)0;           /* D(0) = D(1) = 0 by definition */
    }

    uint64_t facs[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    factor_u64((uint64_t)n, facs, exps, &cnt);   /* factorization */

    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p  = (seq_t)facs[i];
        seq_t v  = (seq_t)exps[i];
        seq_t pi = compute_pi((idx_t)p);       /* π(p) */

        /* term = (n / p) * π(p) * v  */
        seq_t term = (n / p) * pi * v;
        sum += term;
    }
    return sum;
}

/* Seven‑fold application of the π‑based arithmetic derivative. */
seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    for (int i = 0; i < 7; ++i) {
        val = derivative(val);
    }
    return val;
}
