/*
 * A369757  The number of divisors of the smallest cubefull exponentially odd
 *          number that is divisible by n.
 * https://oeis.org/A369757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e || n} ( e+2 if e even , max(e,3)+1 if e odd )
 * Range:     u8 n <= 209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the multiplicative rule describing a(n) from the OEIS entry. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Special case: n = 1 has no prime factors, so a(1) = 1. */
    if (n == (idx_t)1)
        return (seq_t)1;

    /* We will factor n into prime powers.  The helper factor_u64()
       fills the arrays `primes` and `exps` with the distinct primes
       dividing n and their corresponding exponents, and writes the
       number of factors into `cnt`.  Allocate a size that is safely
       large for any 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        seq_t factor;
        if (e & 1ULL) /* odd exponent */
            factor = (seq_t)( (e > 3 ? e : 3) + 1 );
        else /* even exponent */
            factor = (seq_t)(e + 2);
        result *= factor;
    }
    return result;
}
