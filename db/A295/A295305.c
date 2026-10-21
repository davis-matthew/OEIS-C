/*
 * A295305  a(n) = tau(sigma(n)) - tau(n), where tau is the number of divisors
 *          (A000005) and sigma is the sum of divisors of n (A000203).
 * https://oeis.org/A295305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = tau(sigma(n)) - tau(n)
 * Range:     i8 n <= 59, i16 n <= 9299, i32 n <= 9299, i64 n <= 9299,
 *            i128 n <= 9299, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Compute the divisor count τ(n) for a 64‑bit integer n. */
static seq_t _tau_u64(uint64_t n)
{
    uint64_t pr[20];
    uint64_t ex[20];
    size_t   cnt = 0;

    factor_u64(n, pr, ex, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i)
        res *= (seq_t)(ex[i] + 1);

    return res;
}

/* Compute the sum of divisors σ(n) for a 64‑bit integer n. */
static seq_t _sigma_u64(uint64_t n)
{
    uint64_t pr[20];
    uint64_t ex[20];
    size_t   cnt = 0;

    factor_u64(n, pr, ex, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = 0;          /* sum p^0 + p^1 + … + p^e */
        seq_t pow  = 1;          /* current power of p */

        for (uint64_t k = 0; k <= ex[i]; ++k) {
            term += pow;
            pow *= (seq_t)pr[i];
        }
        res *= term;
    }

    return res;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* OEIS offset 1: n ≥ 1. For n == 0 the function is not used. */
    uint64_t N = (uint64_t)n;

    seq_t tau_n    = _tau_u64(N);
    seq_t sigma_n  = _sigma_u64(N);
    seq_t tau_sigma = _tau_u64((uint64_t)sigma_n);

    return tau_sigma - tau_n;
}
