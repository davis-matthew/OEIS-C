/*
 * A365685  a(n) is the smallest number k such that k*n is an exponentially
 *          squarefree number (A209061).
 * https://oeis.org/A365685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = minimum k such that k*n is exponentially squarefree. This
 *            function implements a(n)=∏p p^(f(e)-e) where e is the exponent
 *            of prime p in n and f(e) is the smallest t such that e+t is
 *            squarefree
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static inline int is_squarefree_uint64(uint64_t m)
{
    for (uint64_t r = 2; r * r <= m; ++r) {
        if (m % (r * r) == 0) return 0;
    }
    return 1;
}

static inline uint64_t min_t_squarefree(uint64_t e)
{
    uint64_t t = 0;
    while (!is_squarefree_uint64(e + t)) ++t;
    return t;
}

seq_t a(idx_t n)
{
    /* n is always ≥1 as per OEIS offset 1 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        uint64_t t = min_t_squarefree(e);
        for (uint64_t j = 0; j < t; ++j) {
            result *= (seq_t)primes[i];
        }
    }
    return result;
}
