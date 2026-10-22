/*
 * A382290  a(n) = A064547(n) - A001221(n).
 * https://oeis.org/A382290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = Σ_{p^e || n} (popcount(e) - 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is additive over prime powers and reduces to a(p^e) = popcount(e) - 1 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Compute the value of a(n) = A064547(n) – A001221(n) */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    /* Maximum number of distinct prime factors of a 64‑bit integer is 64 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* Factor n into primes and exponents */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        /* popcount of e */
        unsigned pc = __builtin_popcountll(e);
        /* Add popcount(e) - 1, which is always ≥ 0 for e ≥ 1 */
        sum += (seq_t)(pc - 1);
    }
    return sum;
}
