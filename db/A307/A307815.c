/*
 * A307815  Number of partitions of n into 3 squarefree parts.
 * https://oeis.org/A307815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of partitions of n into 3 square‑free parts (unordered)
 * Range:     u8 n <= 106, u16 n <= 1822, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"

static bool *squarefree_cache = NULL;
static idx_t cache_max = 0;

/* Ensure that squarefree_cache[0..n] is present and correctly marked. */
static void ensure_squarefree_cache(idx_t n)
{
    if (cache_max >= n)
        return;

    /* Allocate or enlarge the cache. */
    size_t new_cap = (size_t)(n + 1);
    bool *new_cache = (bool *)realloc(squarefree_cache, new_cap * sizeof(bool));
    if (!new_cache)
        abort();          /* allocation failed */
    /* Set new entries to true initially. */
    for (size_t i = cache_max + 1; i < new_cap; ++i)
        new_cache[i] = true;
    squarefree_cache = new_cache;

    /* Mark all numbers that are divisible by a square > 1. */
    idx_t limit = floor_isqrt_u64(n);
    size_t prime_cnt;
    uint64_t *primes = NULL;
    build_primes_upto(limit, &primes, &prime_cnt);
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p  = primes[pi];
        uint64_t p2 = p * p;
        for (idx_t j = p2; j <= n; j += p2)
            squarefree_cache[j] = false;
    }
    free(primes);

    cache_max = n;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;           /* cannot partition into three positive parts */

    ensure_squarefree_cache(n);

    seq_t total = 0;

    for (idx_t k = 1; k <= n / 3; ++k) {
        if (!squarefree_cache[k])
            continue;        /* k must be square‑free */

        idx_t max_i = (n - k) / 2;
        for (idx_t i = k; i <= max_i; ++i) {
            if (!squarefree_cache[i])
                continue;    /* i must be square‑free */

            idx_t j = n - k - i;
            if (j < i)
                continue;    /* enforce k <= i <= j */

            if (squarefree_cache[j])
                total += 1;   /* found a valid unordered triple */
        }
    }

    return total;
}
