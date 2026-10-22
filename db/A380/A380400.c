/*
 * A380400  The sum of unitary divisors of n that are perfect powers
 *          (A001597).
 * https://oeis.org/A380400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = Σ_{d|n, gcd(d, n/d)=1} d * [ d is a perfect power ] * =
 *            sum of all unitary divisors of n that are perfect powers. * This
 *            code factors n, enumerates all unitary divisors (one per subset
 *            of * distinct prime factors), checks whether the divisor is a
 *            perfect power * by inspecting the gcd of...
 * Range:     u8 n <= 224, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence undefined for n=0 */

    /* factor n: p[i]^e[i] */
    static uint64_t p[64];
    static uint64_t e[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, p, e, &cnt);

    seq_t sum = 0;
    size_t subsets = (size_t)1 << cnt;     /* cnt is small for n ≤ 10000 */

    for (size_t mask = 0; mask < subsets; ++mask) {
        seq_t d = 1;
        seq_t g = 0;                       /* gcd of chosen exponents */

        for (size_t i = 0; i < cnt; ++i) {
            if (mask & ((size_t)1 << i)) {
                /* multiply by p[i]^e[i] */
                d *= pow_uint(p[i], (idx_t)e[i]);

                /* update gcd of exponents */
                if (g == 0)
                    g = (seq_t)e[i];
                else
                    g = gcd(g, (seq_t)e[i]);
            }
        }

        /* 1 (empty subset) is a perfect power (1^k). Other divisors
           are perfect powers iff the gcd of their exponents > 1. */
        if (mask == 0 || g > 1)
            sum += d;
    }

    return sum;
}
