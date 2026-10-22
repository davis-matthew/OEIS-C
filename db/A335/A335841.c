/*
 * A335841  Number of distinct rectangles that can be made with one even and
 *          one odd side length that are divisors of 2n.
 * https://oeis.org/A335841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = tau(n) * (tau(2*n) - tau(n))
 * Range:     u8 n <= 629, u16 n <= 9936, u32 n <= 9936, u64 n <= 9936,
 *            u128 n <= 9936, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Count the number of divisors of a 64‑bit integer. */
static seq_t tau_u64(uint64_t m)
{
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);
    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        res *= (seq_t)(exps[i] + 1);
    }
    return res;
}

/* OEIS A335841 */
seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    seq_t tau_n  = tau_u64(x);
    uint64_t double_x = x << 1;          /* 2*n */
    seq_t tau_2n = tau_u64(double_x);
    return tau_n * (tau_2n - tau_n);
}
