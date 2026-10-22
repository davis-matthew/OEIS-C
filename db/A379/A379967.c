/*
 * A379967  Arithmetic derivative of {n divided by its largest squarefree
 *          divisor}: a(n) = A003415(A003557(n)).
 * https://oeis.org/A379967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415( n / rad(n) ) * where rad(n) = product of
 *            distinct prime factors of n * (the largest squarefree divisor).
 *            For n = ∏ p_i^{e_i} with e_i ≥ 1, * let m = n / rad(n) = ∏
 *            p_i^{e_i-1} and * a(n) = Σ_{i : e_i > 1} (e_i-1) * m / p_i
 * Range:     u8 n <= 255, u16 n <= 9344, u32 n <= 9344, u64 n <= 9344,
 *            u128 n <= 9344, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* factor n into primes[] and exps[] */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* compute m = n / rad(n) = ∏ p_i^{e_i-1} */
    seq_t m = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        if (e <= 1) continue;          /* no contribution to m */
        uint64_t p = primes[i];
        for (uint64_t j = 0; j < e - 1; ++j) {
            m *= (seq_t)p;
        }
    }

    /* compute arithmetic derivative of m */
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        if (e <= 1) continue;
        uint64_t p = primes[i];
        seq_t term = (m / (seq_t)p) * (seq_t)(e - 1);
        sum += term;
    }

    return sum;
}
