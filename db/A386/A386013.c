/*
 * A386013  a(n) = n^4*tau(n).
 * https://oeis.org/A386013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^4*tau(n)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 139, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* tau(n) is the divisor‑count function A000005 */

#include <stdint.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Compute the number of divisors of n using trial division. */
static seq_t tau_of(uint64_t n)
{
    seq_t res = 1;
    uint64_t m = n;

    /* Count factor 2. */
    if ((m & 1ULL) == 0) {
        uint64_t cnt = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++cnt;
        }
        res *= (cnt + 1);
    }

    /* Count odd factors. */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            uint64_t cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }
            res *= (cnt + 1);
        }
    }

    /* Remaining prime factor. */
    if (m > 1) {
        res *= 2;  /* exponent 1 */
    }

    return res;  /* n >= 1, res >= 1 */
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* Out of sequence range, defined as 0. */
    }

    /* Ensure the cache can hold the requested index. */
    ensure_cache(n, &cache, &cache_cap);

    /* Return memoised value if available. */
    if (cache[n - 1] != 0) {
        return cache[n - 1];
    }

    /* Calculate a(n) = n^4 * tau(n). */
    seq_t nn   = (seq_t)n;
    seq_t tau  = tau_of((uint64_t)n);

    seq_t result = nn * nn;   /* n^2 */
    result      = result * nn; /* n^3 */
    result      = result * nn; /* n^4 */
    result      = result * tau;

    cache[n - 1] = result;
    return result;
}
