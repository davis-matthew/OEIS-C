/*
 * A359590  Absolute values of A355690, where A355690 is the Dirichlet inverse
 *          of the characteristic function of numbers not congruent to 2 mod
 *          4.
 * https://oeis.org/A359590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is multiplicative; a(2^e) = 1 if e mod 3 == 0 or 2,
 *            otherwise 0; for an odd prime p, a(p^e) = 1 if e == 1, otherwise
 *            0
 * Range:     u8 n <= 9307, u16 n <= 9307, u32 n <= 9307, u64 n <= 9307,
 *            u128 n <= 9307, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* defined for safety only */

    uint64_t nn = (uint64_t)n;
    if (nn == 1) return (seq_t)1;          /* a(1) = 1 */

    /* Factor n into primes and exponents.  We rely on the helper
       factor_u64 which writes into the supplied arrays. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == 2ULL) {                   /* power of two */
            if ((e % 3ULL) == 1ULL) return (seq_t)0;
        } else {                            /* odd prime */
            if (e > 1ULL) return (seq_t)0;
        }
    }
    return (seq_t)1;
}
