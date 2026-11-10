/*
 * A353329  Number of divisors d of n for which A332823(d) is negative (-1).
 * https://oeis.org/A353329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [A332823(d) < 0]; A332823(d) == sum of
 *            exponents weighted +1/-1 by parity of prime index (mod 3),
 *            counted by DP over prime powers
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9924 (2026-11-10)
 * Helpers:   factor_u64, prime_pi_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi_u64.h"

seq_t a(idx_t n)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    uint64_t cntr[3] = {1, 0, 0};   /* divisors by residue class of weighted sum mod 3 */

    if (n > 1)
        factor_u64((uint64_t)n, pr, ex, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t idx = prime_pi_u64(pr[i]) - 1;   /* 0-based prime index */
        int w = (idx & 1) ? 2 : 1;                /* -1 == 2 mod 3 */
        uint64_t e = ex[i];
        uint64_t nw[3] = {0, 0, 0};
        for (uint64_t k = 0; k <= e; ++k) {
            int shift = (int)((k % 3) * (uint64_t)w % 3);
            for (int r = 0; r < 3; ++r)
                nw[(r + shift) % 3] += cntr[r];
        }
        for (int r = 0; r < 3; ++r) cntr[r] = nw[r];
    }
    return (seq_t)cntr[2];
}
