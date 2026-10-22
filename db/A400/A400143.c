/*
 * A400143  The number of divisors d of n such that gcid(d, n/d) is a power of
 *          2, where gcid is the greatest common infinitary divisor.
 * https://oeis.org/A400143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = the number of divisors d of n such that gcid(d, n/d) is a
 *            power of 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative:  a(2^e) = e+1,  a(p^e) = 2^popcount(e)  for odd prime p */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n <= 1)          /* a(1) = 1 */
        return 1;

    uint64_t x = (uint64_t)n;
    /* arrays large enough for all distinct prime factors of a 64‑bit integer */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    factor_u64(x, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (pr[i] == 2) {            /* power of 2 contributes e+1 */
            result *= (seq_t)(ex[i] + 1);
        } else {                     /* odd prime contributes 2^popcount(e) */
            unsigned pc = __builtin_popcountll(ex[i]);
            seq_t contrib = pow2(pc);
            result *= contrib;
        }
    }
    return result;
}
