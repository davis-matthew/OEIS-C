/*
 * A363277  Sum of the divisor complements of the squarefree divisors of n
 *          that are <= sqrt(n).
 * https://oeis.org/A363277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * Sum_{d|n, d<=sqrt(n)} mu(d)^2 / d
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   _floor_isqrt_u64, dfs_add, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sum runs over the square‑free divisors d of n that do not exceed sqrt(n). */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/dfs_add.h"

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 1. Calls with n = 0 are undefined; return 0 safely. */
    if (n == 0)
        return 0;

    /* Contribution from the divisor d = 1. */
    seq_t total = (seq_t)n;

    /* Square root floor of n. */
    idx_t sqrt_n = _floor_isqrt_u64(n);

    /* Factor n to obtain its distinct prime factors. */
    uint64_t primes[64];
    uint64_t exps[64];   /* exps are unused except to match the function signature */
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    if (cnt == 0)            /* n == 1 */
        return total;

    /* Add n/d for all other square‑free divisors d <= sqrt(n). */
    dfs_add(n, primes, 0, cnt, 1, (uint64_t)sqrt_n, &total);

    return total;
}
