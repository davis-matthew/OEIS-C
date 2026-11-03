/*
 * A399737  The sum of the unitary divisors d of n such that n/d is a cube.
 * https://oeis.org/A399737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 231, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 8148 (2026-11-01)
 * Helpers:   factor_u64, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative sequence: a(p^e) = p^e + 1 if 3|e, else a(p^e) = p^e.
   The general term is a(n) = ∏_{p^e||n} (p^e + δ_{3|e})   where δ is 1 if 3|e, 0 otherwise. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for 0, return 0 */
    if (n == 1) return (seq_t)1;  /* a(1) = 1 */

    /* factor n; max distinct primes for 64-bit is < 15, so allocate safe buffer */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = (idx_t)exps[i];
        seq_t p_pow = ipow_idx(p, e);

        if (e % 3 == 0) {
            /* include (1 + p^e) */
            result *= (p_pow + (seq_t)1);
        } else {
            /* include p^e */
            result *= p_pow;
        }
    }
    return result;
}
