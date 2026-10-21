/*
 * A245572  Expansion of phi(q) * phi(q^2) + 2 * phi(-q^2) * phi(q^4) in
 *          powers of q where phi() is a Ramanujan theta function.
 * https://oeis.org/A245572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A245572: a(n) = 2 * b(n) where b is a multiplicative function with
   b(2) = -1, b(2^e) = 3 for e > 1,
   b(p^e) = e + 1   if p ≡ 1 or 3 (mod 8),
   b(p^e) = (1 + (-1)^e)/2   if p ≡ 5 or 7 (mod 8).
   a(0) = 3.
*/
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Handle the special offset term */
    if (n == 0)
        return (seq_t)3;

    /* Factor n into prime powers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t b = (seq_t)1;        /* multiplicative identity */

    for (size_t i = 0; i < cnt; ++i) {
        const uint64_t p = primes[i];
        const uint64_t e = exps[i];
        seq_t contrib;

        if (p == 2ULL) {
            if (e == 1ULL)
                contrib = (seq_t)-1;
            else
                contrib = (seq_t)3;
        } else {
            const uint64_t mod8 = p & 7ULL;    /* p % 8 */
            if (mod8 == 1ULL || mod8 == 3ULL) {
                /* (e + 1) fits in seq_t */
                contrib = (seq_t)(e + 1ULL);
            } else {  /* p ≡ 5 or 7 (mod 8) */
                if ((e & 1ULL) == 0)            /* even exponent */
                    contrib = (seq_t)1;
                else                           /* odd exponent */
                    contrib = (seq_t)0;
            }
        }

        b *= contrib;
        if (b == (seq_t)0)      /* early exit: product is zero */
            break;
    }

    return b * (seq_t)2;
}
