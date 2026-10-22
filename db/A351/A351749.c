/*
 * A351749  a(n) = Sum_{p|n, p prime} sigma_p(p).
 * https://oeis.org/A351749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} ( 1 + p^p )
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        /* a(1) = 0, a(0) is not defined but we return 0 for safety */
        return (seq_t)0;
    }

    /* Factor n and obtain distinct prime factors */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t pp = big_pow(p, p);   /* compute p^p */
        result += pp + 1;            /* add (1 + p^p) */
    }

    return result;
}
