/*
 * A319442  Number of divisors of n over the Eisenstein integers.
 * https://oeis.org/A319442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 5459, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = ∏ over prime powers p^e in n of
 *  a(p^e) = 2e+1 if p==3,
 *          (e+1)^2 if p≡1 (mod 3),
 *          (e+1)   if p≡2 (mod 3).
 *  (a(1)=1.)
 */
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    /* arrays large enough for any uint64 factorisation */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;

    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t res = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        seq_t val;

        if (p == 3) {
            val = (seq_t)(2 * e + 1);
        } else if (p % 3 == 1) {
            seq_t t = (seq_t)(e + 1);
            val = t * t;
        } else { /* p % 3 == 2 */
            val = (seq_t)(e + 1);
        }

        res *= val;
    }

    return res;
}
