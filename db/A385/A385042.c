/*
 * A385042  The number of unitary divisors of n whose exponents in their prime
 *          factorizations are all powers of 2 (A138302).
 * https://oeis.org/A385042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (e is a power of 2 ? 2 : 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*      i.e.  a(n) = 2^{# of distinct prime factors of n whose exponent is a power of 2} */

#include <stddef.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

seq_t a(idx_t n)
{
    if (n < 1)      /* undefined for n < 1, but guard for safety */
        return 0;

    /* Maximum number of distinct prime factors of a 64‑bit integer is 64. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        /* Check if e is a power of two: e > 0 and e & (e-1) == 0 */
        if (e && (e & (e - 1)) == 0) {
            res = res * 2;
        }
    }
    return res;
}
