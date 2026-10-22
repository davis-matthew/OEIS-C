/*
 * A321558  a(n) = Sum_{d divides n} (-1)^(d + n/d) * d^2.
 * https://oeis.org/A321558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, look
 * Formula:   a(n) = Sum_{d divides n} (-1)^(d + n/d) * d^2 This
 *            implementation uses the multiplicative closed form: For p odd
 *            prime, a(p^k) = (p^(2k+2) - 1)/(p^2 - 1) For p = 2, a(2^k) = -
 *            (2^(2k+1) + 7)/3 and a(1) = 1
 * Range:     i8 n <= 4, i16 n <= 16, i32 n <= 3208, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static idx_t   sqrt_limit = 0;

static seq_t component_for_prime_power(uint64_t p, size_t exponent)
{
    if (p == 2) {
        /* a(2^k) = - (2^(2k+1) + 7)/3 */
        seq_t term = pow_uint(2U, 2*exponent + 1);
        return - ((term + 7) / 3);
    } else {
        /* a(p^k) = (p^(2k+2) - 1)/(p^2 - 1)  (p odd) */
        seq_t numerator   = pow_uint(p, 2*exponent + 2) - 1;
        seq_t denominator = pow_uint(p, 2) - 1;
        return numerator / denominator;
    }
}

seq_t a(idx_t n)
{
    if (n == 1U) return (seq_t)1;

    idx_t limit = _floor_isqrt_u64(n);
    if (limit > sqrt_limit || prime_list == NULL) {
        free(prime_list);
        build_primes_upto(limit, &prime_list, &prime_cnt);
        sqrt_limit = limit;
    }

    seq_t result = 1;
    idx_t tmp = n;

    for (size_t i = 0; i < prime_cnt && prime_list[i]*prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            size_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            result *= component_for_prime_power(p, exp);
        }
    }
    if (tmp > 1U) {
        /* Remaining prime factor */
        result *= component_for_prime_power(tmp, 1);
    }
    return result;
}
