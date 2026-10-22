/*
 * A368921  a(n) = n' - n'', where n' is the arithmetic derivative of n,
 *          A003415(n) and n'' is its second arithmetic derivative,
 *          A068346(n).
 * https://oeis.org/A368921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A003415(n) - A068346(n)
 * Range:     i8 n <= 63, i16 n <= 2047, i32 n <= 9904, i64 n <= 9904,
 *            i128 n <= 9904, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the arithmetic derivative and its derivative for each n */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static seq_t arithmetic_derivative_uint64(uint64_t n)
{
    if (n <= 1)
        return (seq_t)0;

    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t res = 0;
    const seq_t tmp = (seq_t)n;

    for (size_t i = 0; i < cnt; ++i) {
        const uint64_t p = primes[i];
        const uint64_t e = exps[i];
        /* term = e * (n / p) */
        seq_t term = (seq_t)e * (tmp / (seq_t)p);
        res += term;
    }
    return res;
}

seq_t a(idx_t n)
{
    const uint64_t n64 = (uint64_t)n;
    const seq_t n_prime = arithmetic_derivative_uint64(n64);
    const seq_t n_second = arithmetic_derivative_uint64((uint64_t)n_prime);
    return n_prime - n_second;
}
