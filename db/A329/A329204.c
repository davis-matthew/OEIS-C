/*
 * A329204  Number of integers less than n having at least as many totatives
 *          as n.
 * https://oeis.org/A329204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = #{m < n | φ(m) ≥ φ(n)}
 * Range:     u8 n <= 509, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for totient values as uint64_t. */
static uint64_t *phi_cache = NULL;
static size_t    phi_cache_size = 0;

/* Ensure that phi_cache[0..limit] contains φ(0)..φ(limit). */
static void ensure_phi(idx_t limit)
{
    /* Grow the cache if necessary. */
    if (phi_cache == NULL || (size_t)limit + 1 > phi_cache_size)
    {
        size_t new_size = (size_t)limit + 1;
        uint64_t *tmp = (uint64_t *)realloc(phi_cache, new_size * sizeof(uint64_t));
        if (tmp == NULL) abort();
        /* Initialize new entries to 0. */
        for (size_t i = phi_cache_size; i < new_size; ++i) tmp[i] = 0;
        phi_cache = tmp;
        phi_cache_size = new_size;
    }

    /* Compute Euler's totient function up to limit using the classic
       sieve O(n log log n).  The array already contains data for smaller
       indices; we recompute from scratch for simplicity and correctness. */
    for (uint64_t i = 0; i <= limit; ++i) phi_cache[i] = i;
    for (uint64_t p = 2; p <= limit; ++p)
    {
        if (phi_cache[p] == p)          /* p is prime */
        {
            for (uint64_t j = p; j <= limit; j += p)
                phi_cache[j] -= phi_cache[j] / p;
        }
    }
}

/* Main function that computes the A329204 sequence. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;           /* Undefined for n=0 in OEIS. */

    ensure_phi(n);

    uint64_t phi_n = phi_cache[n];
    uint64_t count = 0;

    for (uint64_t i = 1; i < n; ++i)
        if (phi_cache[i] >= phi_n) ++count;

    return (seq_t)count;
}
