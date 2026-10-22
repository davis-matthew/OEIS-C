/*
 * A366765  The largest divisor of n that have no exponent 2 in their prime
 *          factorization.
 * https://oeis.org/A366765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e || n} (p if e <= 2 else p^e)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* small power helper: base^exp computed in seq_t */
static inline seq_t pow_seq(uint64_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        res *= (seq_t)base;
    return res;
}

seq_t a(idx_t n)
{
    uint64_t n64 = (uint64_t)n;
    if (n64 <= 1)
        return (seq_t)1;          /* a(1) = 1, a(0) is unused */

    /* Factorization buffers – 64 distinct primes is ample for a 64‑bit n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n64, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] <= 2)
            result *= (seq_t)primes[i];
        else
            result *= pow_seq(primes[i], (idx_t)exps[i]);
    }
    return result;
}
