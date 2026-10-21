/*
 * A254315  Number of distinct digits in the prime factorization of n
 *          (counting terms of the form p^1 as p).
 * https://oeis.org/A254315
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   number of distinct digits in the prime factorization of n
 *            (digits of all primes and digits of all exponents > 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    /* factor_u64 will fill these arrays with the prime factors and
       their exponents of n.  The maximum number of distinct primes for
       a 64‑bit integer is below 64, so 128 is safe. */
    uint64_t primes[128];
    uint64_t exps[128];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    bool seen[10] = { false };
    /* collect digits from primes */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        while (p > 0) {
            seen[p % 10] = true;
            p /= 10;
        }
    }
    /* collect digits from exponents > 1 */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        if (e > 1) {
            while (e > 0) {
                seen[e % 10] = true;
                e /= 10;
            }
        }
    }
    /* count distinct digits */
    seq_t result = 0;
    for (int d = 0; d < 10; ++d)
        if (seen[d]) ++result;
    return result;
}
