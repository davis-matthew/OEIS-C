/*
 * A342023  a(n) = 1 if there is a prime p such that p^p divides n, otherwise
 *          0.
 * https://oeis.org/A342023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [∃prime p : p^p divides n] (computes by factoring n)
 * Range:     u8 n <= 9947, u16 n <= 9947, u32 n <= 9947, u64 n <= 9947,
 *            u128 n <= 9947, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* The index type satisfies idx_t == uint64_t */
    uint64_t val = (uint64_t)n;

    /* Factorize val into primes and exponents */
    /* 20 distinct primes are more than enough for 64‑bit numbers */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t count = 0;

    factor_u64(val, primes, exps, &count);

    for (size_t i = 0; i < count; ++i) {
        /* p^p divides val iff exponent of p (exps[i]) is at least p */
        if (exps[i] >= primes[i]) {
            return (seq_t)1;
        }
    }

    return (seq_t)0;
}
