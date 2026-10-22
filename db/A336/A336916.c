/*
 * A336916  One more than the number of iterations of x -> 2x needed before
 *          the result is nondeficient, when starting from x=n; a(n) = 0, when
 *          n is a power of 2.
 * https://oeis.org/A336916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1542, u32 n <= 9720, u64 n <= 9720,
 *            u128 n <= 9720, big any n
 * Helpers:   factor_u64, is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A336916: a(n) = 0 if n is a power of two; otherwise
 * a(n) = 1 + min{k ≥ 0 | sigma(2^k * n) ≥ 2 * (2^k * n)} */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/is_power_of_two.h"

/* Compute σ(m) for m ≤ 2^64‑1 using its prime factorisation. */
static seq_t sigma_u64(uint64_t m)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        size_t e = ex[i];

        /* Compute (p^(e+1) – 1) / (p – 1) using only seq_t arithmetic. */
        seq_t num = 1;
        for (size_t j = 0; j < e + 1; ++j) {
            num *= p;
        }
        num -= 1;
        seq_t denom = p - 1;
        seq_t term = num / denom;

        result *= term;
    }
    return result;
}

/* Return true if x is non‑deficient (σ(x) ≥ 2x). */
static bool is_nondeficient(seq_t x)
{
    /* We only ever call this with x ≤ 2^64‑1. */
    uint64_t ux = (uint64_t)x;
    seq_t sigma = sigma_u64(ux);
    seq_t two_x = x * (seq_t)2;
    return sigma >= two_x;
}

/* OEIS sequence A336916. */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;

    /* By definition a power of two has value 0. */
    if (is_power_of_two(sn))
        return 0;

    seq_t current = sn;
    unsigned int k = 0;

    /* Find the first doubling that yields a non‑deficient number. */
    while (!is_nondeficient(current)) {
        current *= (seq_t)2;
        ++k;
    }

    /* One more than the number of iterations needed. */
    return (seq_t)(k + 1);
}
