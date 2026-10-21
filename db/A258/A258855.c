/*
 * A258855  Fifth pi-based arithmetic derivative of n.
 * https://oeis.org/A258855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A258851^5(n) where A258851 is the pi‑based arithmetic
 *            derivative: For n = ∏ p_i^e_i dπ(n) = n * Σ ( e_i * π(p_i) / p_i
 *            ) (π(x) = number of primes ≤ x) a(n) is the fifth iterate of dπ
 *            applied to n
 * Range:     u8 n <= 11, u16 n <= 79, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/compute_pi.h"

/* compute the pi‑based arithmetic derivative of a positive integer */
static seq_t pi_derivative(seq_t n) {
    if (n <= 1) return 0;

    /* factorization: n fits in 64‑bit for all smaller inputs */
    uint64_t p[64], e[64];
    size_t cnt;
    factor_u64((uint64_t)n, p, e, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        /* term = (n / p_i) * e_i * π(p_i) */
        seq_t term = (n / (seq_t)p[i]) * (seq_t)e[i] * compute_pi(p[i]);
        sum += term;
    }
    return sum;
}

/* main sequence function required by the build */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t val = (seq_t)n;
    for (int i = 0; i < 5; ++i) {
        val = pi_derivative(val);
        if (val == 0) break;      /* optional early stop */
    }
    return val;
}
