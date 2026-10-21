/*
 * A257196  Expansion of (1 + x) * (1 + x^5) / ((1 + x^2) * (1 + x^4)) in
 *          powers of x.
 * https://oeis.org/A257196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   multiplicative; a(0)=1, a(2)=-1, a(2^e)=0 (e>1), a(p^e)=1 if p≡1
 *            (mod 4), a(p^e)=(-1)^e if p≡3 (mod 4)
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* provides void factor_u64(uint64_t n,
                                      uint64_t *pr, uint64_t *ex, size_t *cnt); */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }

    /* Factorization buffer. 64 distinct primes are enough for any uint64_t. */
    static const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt = 0;

    /* factor_u64 expects n>0. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t val;

        if (p == 2) {
            if (e == 1) {
                val = (seq_t)(-1);
            } else {
                /* a(2^e)=0 for e>1 */
                return (seq_t)0;
            }
        } else {
            /* p is an odd prime.  p % 4 is either 1 or 3. */
            if ((p % 4) == 1) {
                val = (seq_t)1;
            } else { /* p ≡ 3 (mod 4) */
                if ((e % 2) == 0) {
                    val = (seq_t)1;
                } else {
                    val = (seq_t)(-1);
                }
            }
        }

        result *= val;
        /* If any factor caused result to become 0, return early. */
        if (result == (seq_t)0) {
            return result;
        }
    }

    return result;
}
