/*
 * A378905  a(n) is the number of odd squarefree semiprimes (A046388) <
 *          prime(n).
 * https://oeis.org/A378905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 214, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of odd squarefree semiprimes < prime(n). */
/* Prime generation and counting are done lazily and cached for future calls. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Cached sequence values up to the largest requested n. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    if (n == 0)          /* Undefined for n = 0; return 0. */
        return 0;

    /* Return immediately if we already have the value. */
    if (n <= cache_cap)
        return cache[n - 1];

    /* ------------------------------------------------------------------
     *  Compute a(k) for all k = 1 .. n.
     *  ------------------------------------------------------------------ */

    /* 1. Determine the required prime bound. */
    uint64_t limit = (uint64_t)get_prime(n);          /* prime(n) */

    /* 2. Build list of all primes ≤ limit. */
    size_t prime_cnt = 0;
    uint64_t *primes = NULL;
    build_primes_upto(limit, &primes, &prime_cnt);
    if (prime_cnt == 0) {                            /* safety check */
        return 0;
    }

    /* 3. Mark all odd square‑free semiprimes ≤ limit. */
    uint8_t *flag = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (!flag) {
        free(primes);
        return 0;
    }

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t pi = primes[i];
        if (pi == 2)               /* skip even prime */
            continue;
        for (size_t j = i + 1; j < prime_cnt; ++j) {
            uint64_t pj = primes[j];
            uint64_t prod = pi * pj;
            if (prod > limit)
                break;
            flag[(size_t)prod] = 1;
        }
    }

    /* 4. Compute prefix sums of the flag array. */
    seq_t *prefix = (seq_t *)malloc(sizeof(seq_t) * (limit + 1));
    if (!prefix) {
        free(flag);
        free(primes);
        return 0;
    }
    prefix[0] = 0;
    for (size_t i = 1; i <= limit; ++i)
        prefix[i] = prefix[i - 1] + (seq_t)flag[i];

    /* 5. Allocate new cache and fill it. */
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * n);
    if (!new_cache) {
        free(flag);
        free(prefix);
        free(primes);
        return 0;
    }

    /* a(k) = number of semiprimes < prime(k). */
    for (size_t k = 1; k <= n; ++k) {
        uint64_t pk = primes[k - 1];
        new_cache[k - 1] = prefix[pk - 1];
    }

    /* 6. Clean up temporary data. */
    free(flag);
    free(prefix);
    free(primes);

    /* Replace old cache (if any) with the new one. */
    free(cache);
    cache     = new_cache;
    cache_cap = n;

    return cache[n - 1];
}
