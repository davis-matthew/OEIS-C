/*
 * A368883  The number of infinitary divisors of n that are cubefree.
 * https://oeis.org/A368883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏p^e||n f(p^e) , where f(p^e) = 1 if e ≡ 0 (mod 4),
 *            f(p^e) = 2 if e ≡ 1 or 2 (mod 4), f(p^e) = 3 if e ≡ 3 (mod 4).
 *            a(1) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper for factoring 64‑bit integers */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined for n=0, return 0 */
    uint64_t m = (uint64_t)n;
    if (m == 1u)
        return 1;               /* a(1) = 1 */

    /* Array sizes are ample for any 64‑bit integer (max 16 distinct primes,
       64 is safely overboard). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(m, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e  = exps[i];
        uint64_t r  = e & 3ULL;           /* e % 4 */
        seq_t mult  = (r == 0) ? 1
                               : (r == 1 || r == 2 ? 2 : 3);
        res *= mult;
    }
    return res;
}
