/*
 * A326040  a(n) = sigma(n) - A008833(sigma(n)).
 * https://oeis.org/A326040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) - A008833(sigma(n)) where A008833(m) is the
 *            largest perfect square divisor of m
 * Range:     u8 n <= 107, u16 n <= 9544, u32 n <= 9544, u64 n <= 9544,
 *            u128 n <= 9544, big any n
 * Helpers:   pow_uint, seq_mul, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/pow_uint.h"
#include "../utils/seq_mul.h"

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
                seq_t p_pow = pow_uint(d, exp_sq);
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
