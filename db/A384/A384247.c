/*
 * A384247  The number of integers from 1 to n whose largest divisor that is
 *          an infinitary divisor of n is 1.
 * https://oeis.org/A384247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e || n} (p^e - p^{e-2^⌊v2(e)⌋})
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative; for a prime power p^e, a(p^e) = p^e - p^{e-2^v2(e)}. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety for 0, though a(1) is first term */

    /* factors of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];

        /* compute t = 2^v2(e) */
        uint64_t v  = 0;
        uint64_t temp = e;
        while ((temp & 1ULL) == 0ULL) { temp >>= 1; ++v; }
        uint64_t t = 1ULL << v;          /* t = 2^v2(e) */

        /* big_pow works with seq_t; cast exponents appropriately */
        seq_t p_pow_e      = big_pow((seq_t)p, (seq_t)e);
        seq_t p_pow_diff   = big_pow((seq_t)p, (seq_t)(e - t));
        seq_t factor = p_pow_e - p_pow_diff;

        result *= factor;
    }

    return result;
}
