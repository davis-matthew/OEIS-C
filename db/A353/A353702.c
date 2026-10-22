/*
 * A353702  Composite k such that tau(k') = (tau(k))', where tau(k) is the
 *          number of divisors of k (A000005) and k' is the arithmetic
 *          derivative of k (A003415).
 * https://oeis.org/A353702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   factor_u64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A353702:
   a(n) = the nth composite integer k such that τ(k′) = (τ(k))′
   where τ is the divisor‑count function and k′ is the arithmetic derivative. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
/* helper functions from the utils library */
#include "../utils/factor_u64.h"
#include "../utils/is_prime_small.h"

/* compute τ(n) for a 64‑bit integer n */
static uint64_t tau_uint64(uint64_t n)
{
    if (n == 1) return 1;
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);
    __uint128_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        prod *= (__uint128_t)(ex[i] + 1);
    }
    return (uint64_t)prod;
}

/* compute the arithmetic derivative of a 64‑bit integer n */
static uint64_t derivative_uint64(uint64_t n)
{
    if (n <= 1) return 0;
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);
    __uint128_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        __uint128_t term = (__uint128_t)n / pr[i] * ex[i];
        sum += term;
    }
    return (uint64_t)sum;
}

/* test whether a composite k satisfies τ(k′) = (τ(k))′ */
static bool good_k(uint64_t k)
{
    /* skip primes */
    if (is_prime_small(k)) return false;

    uint64_t tau_k = tau_uint64(k);
    uint64_t tau_k_der = derivative_uint64(tau_k);

    uint64_t k_der = derivative_uint64(k);
    uint64_t tau_k_der2 = tau_uint64(k_der);

    return tau_k_der2 == tau_k_der;
}

/* main OEIS sequence function */
seq_t a(idx_t n)
{
    /* search until the nth valid composite is found */
    uint64_t candidate = 4;          /* smallest composite */
    uint64_t found = 0;
    while (found < n) {
        if (good_k(candidate)) {
            ++found;
            if (found == n) {
                return (seq_t)candidate;
            }
        }
        ++candidate;
    }
    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}
