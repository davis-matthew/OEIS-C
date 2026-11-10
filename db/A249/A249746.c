/*
 * A249746  Permutation of natural numbers: a(n) = A126760(A249735(n)) =
 *          A249824(A064216(n)).
 * https://oeis.org/A249746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + f(A003961(2n-1)), f(m) = 2*floor(m/6) + ((m mod
 *            6)-1)/4
 * Range:     u8 n <= 31, u16 n <= 3280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9113 (2026-11-07)
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

static seq_t A249746_pow(seq_t base, unsigned int e)
{
    seq_t result = 1;
    while (e) {
        if (e & 1U)
            result *= base;
        e >>= 1U;
        if (e)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    uint64_t t = (uint64_t)((unsigned __int128)n * 2u - 1u);

    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(t, primes, exps, &cnt);

    /* m = A003961(2n-1) = A249735(n) */
    seq_t m = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t pn = next_prime(primes[i]);
        m *= A249746_pow((seq_t)pn, (unsigned int)exps[i]);
    }

    seq_t q = m / 6;
    seq_t r = m % 6;
    seq_t res = (seq_t)2 * q + 1;
    if (r == 5)
        res += 1;
    return res;
}
