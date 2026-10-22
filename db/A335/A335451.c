/*
 * A335451  Number of permutations of the prime indices of n with all equal
 *          parts contiguous and none appearing more than twice.
 * https://oeis.org/A335451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001221(n)! if n is cubefree, else 0 A001221(n) is the
 *            number of distinct prime factors of n. A value is cubefree iff
 *            every prime exponent in its factorisation is at most 2
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   factor_u64, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/factorial.h"   /* seq_t factorial(idx_t n); */

seq_t a(idx_t n)
{
    /* Handle n = 0?  The offset is 1, but we return 0 for n < 1 for safety. */
    if (n == 0) return 0;

    /* Factorise n.  We need space for primes and exponents.  64 entries are plenty for
       any 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* If any exponent is >= 3, n is not cubefree -> result is 0. */
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] >= 3) return (seq_t)0;
    }

    /* Count of distinct prime factors is cnt.  Return factorial(cnt). */
    return factorial((idx_t)cnt);
}
