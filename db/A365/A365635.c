/*
 * A365635  The largest divisor of n that is a term of A048102.
 * https://oeis.org/A365635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (e < p ? 1 : p^p) (multiplicative definition)
 * Range:     u8 n <= 3124, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;                 /* a(1)=1, no smaller indices matter */

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);  /* factorize n */

    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        if (e >= p) {                     /* exponent meets the condition p^p | n */
            seq_t val = big_pow((seq_t)p, (seq_t)p);  /* compute p^p */
            prod = prod * val;                 /* multiplicative */
        }
    }
    return prod;
}
