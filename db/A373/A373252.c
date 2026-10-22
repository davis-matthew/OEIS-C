/*
 * A373252  a(n) = 1 if the exponent of 2 in their canonical prime
 *          factorization is equal to the maximal exponent, otherwise 0.
 * https://oeis.org/A373252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the exponent of 2 in n equals the maximal exponent
 *            among all prime factors of n; otherwise 0
 * Range:     u8 n <= 9769, u16 n <= 9769, u32 n <= 9769, u64 n <= 9769,
 *            u128 n <= 9769, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>           /* for size_t */

/* Helper to factor a 64‑bit integer into primes and exponents.
   The header and function signature are provided by the build. */
#include "../utils/factor_u64.h"

/* Maximum number of distinct prime factors expected for 64‑bit values. */
#define MAX_FACTORS 64

seq_t a(idx_t n)
{
    /* Convert index to a 64‑bit unsigned integer. */
    uint64_t m = (uint64_t)n;

    /* Special case: n == 1 → exponent of 2 is 0 and max exponent is 0. */
    if (m == 1)
        return (seq_t)1;

    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt = 0;

    /* Factor the number. */
    factor_u64(m, primes, exps, &cnt);

    uint64_t e2 = 0;            /* exponent of 2 */
    uint64_t max_exp = 0;       /* maximum exponent among all primes */

    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] == 2ULL)
            e2 = exps[i];
        if (exps[i] > max_exp)
            max_exp = exps[i];
    }

    return (seq_t)((e2 == max_exp) ? 1 : 0);
}
