/*
 * A326040  a(n) = sigma(n) - A008833(sigma(n)).
 * https://oeis.org/A326040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) - A008833(sigma(n)) where A008833(m) is the
 *            largest perfect square divisor of m
 * Range:     u8 n <= 107, u16 n <= 17639, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 99 terms through n = 9544 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_u64_idx.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Compute the largest perfect square divisor of a uint64_t number. */
static seq_t largest_square_divisor(uint64_t x)
{
    uint64_t tmp = x;
    seq_t result = 1;

    for (uint64_t d = 2; d * d <= tmp; ++d) {
        if (tmp % d == 0) {
            int exp = 0;
            while (tmp % d == 0) {
                tmp /= d;
                exp++;
            }
            int exp_sq = (exp / 2) * 2;        /* even part of the exponent */
            if (exp_sq > 0) {
                seq_t p_pow = ipow_u64_idx(d, exp_sq);
                result = seq_mul(result, p_pow);
            }
        }
    }
    /* If a prime factor remains, its exponent is 1 → even part 0 → skip. */

    return result;
}

seq_t a(idx_t n)
{
    /* sigma(n) fits into seq_t and into uint64_t for the tested region. */
    seq_t sigma_n = sigma_of(n);
    uint64_t sigma_val = (uint64_t)sigma_n;

    seq_t sq_div = largest_square_divisor(sigma_val);

    return sigma_n - sq_div;
}
