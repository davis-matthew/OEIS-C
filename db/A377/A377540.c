/*
 * A377540  Numbers k such that at least one of the numbers 6k-1 or 6k+1 is
 *          prime.
 * https://oeis.org/A377540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by incremental search: test k until the n‑th one is found. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Simple trial‑division primality test for 64‑bit unsigned integers. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1u) == 0) return false;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Return the n-th element of the sequence A377540. */
seq_t a(idx_t n)
{
    /* Memoization state. */
    static idx_t *k_cache      = NULL;   /* cached k values          */
    static size_t k_cache_cap  = 0;      /* allocated capacity       */
    static size_t k_cache_size = 0;      /* number of filled slots   */
    static idx_t next_k        = 1;      /* next k to test           */

    /* safety for the offset 1 (not used in tests) */
    if (n == 0) return 0;

    /* Grow cache until it holds at least n values. */
    while (k_cache_size < n)
    {
        /* Allocate more space if needed. */
        if (k_cache_size == k_cache_cap)
        {
            size_t new_cap = k_cache_cap ? k_cache_cap * 2 : 8;
            idx_t *tmp = (idx_t *)realloc(k_cache, new_cap * sizeof(idx_t));
            if (!tmp) abort();           /* out of memory */
            k_cache = tmp;
            k_cache_cap = new_cap;
        }

        /* Test the next integer k. */
        uint64_t v1 = 6ULL * (uint64_t)next_k - 1ULL;
        uint64_t v2 = 6ULL * (uint64_t)next_k + 1ULL;
        if (is_prime_uint64(v1) || is_prime_uint64(v2))
        {
            k_cache[k_cache_size++] = next_k;
        }
        ++next_k;
    }

    /* Return the cached value cast to the required type. */
    return (seq_t)k_cache[(size_t)(n - 1)];
}
