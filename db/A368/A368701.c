/*
 * A368701  a(n) = 1 if n satisfies the arithmetic differential equation 2n" -
 *          n' - 4 = 0, otherwise 0. Cf. A003415.
 * https://oeis.org/A368701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if 2*n'' – n' – 4 == 0, otherwise 0 (with n' and n''
 *            the arithmetic derivatives). The function computes the
 *            derivatives using prime factorization of the integer n, which is
 *            sufficient for n up to 10 000 (and thus for all tests)
 * Range:     u8 n <= 9442, u16 n <= 9442, u32 n <= 9442, u64 n <= 9442,
 *            u128 n <= 9442, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

static uint64_t derivative_uint64(uint64_t x)
{
    if (x <= 1) return 0;

    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(x, primes, exps, &cnt);

    uint64_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        /* term = (x / p) * e  */
        sum += (x / p) * e;
    }
    return sum;
}

seq_t a(idx_t n)
{
    uint64_t n_val = (uint64_t)n;
    uint64_t nprime = derivative_uint64(n_val);
    uint64_t nprimeprime = derivative_uint64(nprime);

    __uint128_t lhs = (__uint128_t)2 * nprimeprime;
    __uint128_t rhs = (__uint128_t)nprime + 4;

    return (lhs == rhs) ? 1 : 0;
}
