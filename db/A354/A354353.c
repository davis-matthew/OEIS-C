/*
 * A354353  a(n) = 1 if n is either a squarefree composite or a power of
 *          squarefree composite, otherwise 0.
 * https://oeis.org/A354353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff n is a squarefree composite or a power of a
 *            squarefree composite, else 0. This is equivalent to: n has at
 *            least two distinct prime factors and all prime exponents in the
 *            factorisation are equal
 * Range:     u8 n <= 9873, u16 n <= 9873, u32 n <= 9873, u64 n <= 9873,
 *            u128 n <= 9873, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;      /* n=1 is not a composite */
    uint64_t nn = (uint64_t)n;

    /* Static arrays large enough for factorisation of a 64‑bit integer
       (max number of distinct prime factors is < 64). */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    if (cnt < 2) return (seq_t)0;     /* not composite or only one prime factor */

    /* All exponents must be identical */
    uint64_t e0 = exps[0];
    for (size_t i = 1; i < cnt; ++i) {
        if (exps[i] != e0) return (seq_t)0;
    }
    return (seq_t)1;
}
