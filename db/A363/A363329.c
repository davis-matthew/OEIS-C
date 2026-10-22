/*
 * A363329  a(n) is the number of divisors of n that are both coreful and
 *          infinitary.
 * https://oeis.org/A363329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} (2^{popcount(e)} – 1) (multiplicative)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/factor_u64.h"

/* popcount for a 64‑bit integer */
static inline int popcount_u64(uint64_t v)
{
    int c = 0;
    while (v) {
        c += v & 1ULL;
        v >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1)  /* 1 has no prime factors; a(1)=1 */
        return (seq_t)1;

    uint64_t temp = (uint64_t)n;
    /* at most 64 distinct prime factors for a 64‑bit number */
    uint64_t primes[64], exps[64];
    size_t cnt;

    factor_u64(temp, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t exp = exps[i];
        int pc = popcount_u64(exp);
        seq_t factor = pow2((idx_t)pc);   /* 2^pc */
        factor = factor - (seq_t)1;        /* 2^pc – 1 */
        result = result * factor;
    }
    return result;
}
