/*
 * A399092  The maximum exponent in the prime factorization of the
 *          nonsquarefree numbers k such that k+1 is also nonsquarefree.
 * https://oeis.org/A399092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the maximum prime exponent of the nth * nonsquarefree
 *            integer k such that k+1 is also nonsquarefree
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Utility for prime factorisation of uint64_t numbers.
 * The implementation is supplied externally; include its header. */
#include "../utils/factor_u64.h"

/* Helper: check if n is nonsquarefree (has a squared prime factor). */
static bool is_nonsquarefree(uint64_t n)
{
    /* Buffers large enough for any number that can appear in the
     * required range (typically far fewer than 64 distinct primes). */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;

    factor_u64(n, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] >= 2)
            return true;
    return false;
}

/* Helper: maximum exponent in the prime factorisation of n. */
static seq_t max_exponent(uint64_t n)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;

    factor_u64(n, pr, ex, &cnt);
    seq_t maxexp = 1;
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] > maxexp)
            maxexp = (seq_t)ex[i];
    return maxexp;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for 0, but harmless */

    /* Internal static state for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t found_terms = 0;
    static uint64_t processed_k = 1;   /* next k to test */

    /* Generate terms until we have at least n of them. */
    while (found_terms < (size_t)n)
    {
        uint64_t k = processed_k++;

        if (!is_nonsquarefree(k))
            continue;
        if (!is_nonsquarefree(k + 1))
            continue;

        seq_t val = max_exponent(k);

        /* Grow the cache if necessary. */
        if (cache_size == 0)
        {
            cache_size = 16;
            cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        }
        else if (found_terms >= cache_size)
        {
            cache_size <<= 1;
            cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        }

        cache[found_terms++] = val;
    }

    return cache[n - 1];
}
