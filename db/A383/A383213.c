/*
 * A383213  a(n) = number of distinct prime factors of binomial(2n,n+1).
 * https://oeis.org/A383213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct prime factors of binomial(2n, n+1)
 * Range:     u8 n <= 1284, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;
static size_t   primes_count = 0;
static idx_t    primes_limit = 0;

seq_t a(idx_t n)
{
    /* Handle indices less than 1 – they are not part of the sequence. */
    if (n < 1) return (seq_t)0;

    idx_t lim = 2 * n;

    /* Grow the prime list if necessary. */
    if (primes == NULL || lim > primes_limit) {
        size_t cnt = 0;
        uint64_t *new_primes = NULL;
        build_primes_upto(lim, &new_primes, &cnt);

        /* Free old storage, if any. */
        if (primes != NULL) free(primes);

        primes       = new_primes;
        primes_count = cnt;
        primes_limit = lim;
    }

    uint64_t count = 0;

    for (size_t i = 0; i < primes_count; ++i) {
        uint64_t p = primes[i];
        if (p > lim) break;

        uint64_t e = 0;
        idx_t m;

        /* exponent of p in (2n)! */
        m = lim;
        while (m) { m /= p; e += m; }

        /* subtract exponent of p in (n+1)! */
        m = n + 1;
        while (m) { m /= p; e -= m; }

        /* subtract exponent of p in (n-1)! */
        m = n - 1;
        while (m) { m /= p; e -= m; }

        if (e > 0) ++count;
    }

    return (seq_t)count;
}
