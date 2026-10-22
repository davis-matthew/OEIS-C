/*
 * A384251  The number of integers k from 1 to n such that the greatest
 *          divisor of k that is an infinitary divisor of n is odd.
 * https://oeis.org/A384251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative; a(2^e) = 2^e * (1 - 1 / 2^{2^{v₂(e)}}), a(p^e) =
 *            p^e for odd prime p
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

/* Count trailing zeros of a positive 64‑bit integer. */
static inline uint64_t trailing_zeros_u64(uint64_t x)
{
    uint64_t t = 0;
    while (((x >> t) & 1) == 0)
        ++t;
    return t;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 for OEIS indexing.  */
    if (n == 1)
        return 1;

    /* Factor n.  We allocate enough space for all prime factors of a 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == 2) {
            /* e ≥ 1 */
            uint64_t t   = trailing_zeros_u64(e);       /* v₂(e) */
            uint64_t L   = (uint64_t)1 << t;            /* 2^{v₂(e)} */
            uint64_t pow2e       = ((uint64_t)1) << e;  /* 2^e */
            uint64_t pow2e_minus = ((uint64_t)1) << (e - L); /* 2^{e-L} */
            uint64_t val64 = pow2e - pow2e_minus;      /* a(2^e)  */
            result *= (seq_t)val64;
        } else {
            /* odd prime powers contribute p^e */
            seq_t val = pow_uint(p, e); /* p^e, already in seq_t */
            result *= val;
        }
    }

    return result;
}
