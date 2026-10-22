/*
 * A348944  a(n) = (1/2) * (A003959(n)+A034448(n)), where A003959 is
 *          multiplicative with a(p^e) = (p+1)^e and A034448 (usigma) is
 *          multiplicative with a(p^e) = (p^e)+1.
 * https://oeis.org/A348944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A003959(n) + A034448(n)) / 2
 * Range:     u8 n <= 47, u16 n <= 2047, u32 n <= 9837, u64 n <= 9837,
 *            u128 n <= 9837, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

/* computed via prime factorization; multiplicative functions
   with f(p^e) = (p + 1)^e and g(p^e) = p^e + 1.   */
seq_t a(idx_t n)
{
    uint64_t num = (uint64_t)n;          /* uint64_t for factor_u64 */
    size_t cnt;

    /* small static arrays – enough for any 64‑bit integer */
    static uint64_t primes[32], exps[32];

    factor_u64(num, primes, exps, &cnt);

    seq_t f = 1, g = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t e = (seq_t)exps[i];

        seq_t pplus1_pow = big_pow((seq_t)(primes[i] + 1), e);
        f *= pplus1_pow;

        seq_t p_pow = big_pow(p, e);
        g *= (p_pow + (seq_t)1);
    }

    return (f + g) / (seq_t)2;
}
