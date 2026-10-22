/*
 * A309760  Even numbers k such that k-p is composite where p is the largest
 *          prime <= k.
 * https://oeis.org/A309760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 5664, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the n‑th even k such that k − p(k) is composite,
 * where p(k) is the largest prime ≤ k.  The sequence starts
 * 122, 148, 190, …  (OEIS A309760).  This implementation
 * generates terms on demand, caching results for arbitrary
 * ordering of calls.  It uses a simple trial‑division primality
 * test appropriate for the range of values needed (≈10‑thousands
 * terms). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- primality test ---------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    for (uint64_t d = 3ULL; d <= n / d; d += 2ULL) {
        if (n % d == 0ULL) return false;
    }
    return true;
}

/* ---------- cache for the sequence ---------- */
static seq_t *cache   = NULL;   /* array of computed terms      */
static size_t cache_len  = 0;   /* how many terms stored       */
static size_t cache_cap  = 0;   /* allocated capacity          */

/* Ensure that the cache can hold at least new_cap terms. */
static void cache_ensure(size_t new_cap)
{
    if (cache_cap >= new_cap) return;
    size_t sz = cache_cap ? cache_cap * 2 : 16;
    while (sz < new_cap) sz *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, sz * sizeof(seq_t));
    if (!tmp) abort();            /* fatal error if memory exhausted */
    cache   = tmp;
    cache_cap = sz;
}

/* ---------- generate next terms on demand ---------- */
static uint64_t next_candidate = 2ULL;   /* first even number to test */

/* Return the n‑th element of A309760.  n ≥ 1.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety for n < 1 */

    /* adjust target to size_t for the loop.  The sequence will
       never required more than the limits of size_t for the problem
       constraints. */
    const size_t target = (size_t)n;

    /* Generate terms until we have enough cached. */
    while (cache_len < target) {
        uint64_t k = next_candidate;         /* candidate even number */

        /* Find largest prime ≤ k.  Since k is even, the first
           odd ≤ k is k-1, then k-3, ...  */
        uint64_t p = k - 1ULL;
        while (p > 1ULL && !is_prime_uint64(p)) p -= 2ULL; /* skip evens */

        /* If we found a prime, compute diff = k - p. */
        if (p >= 2ULL) {
            uint64_t diff = k - p;
            if (diff > 1ULL && !is_prime_uint64(diff)) {
                /* k satisfies the condition – store it. */
                cache_ensure(cache_len + 1);
                cache[cache_len++] = (seq_t)k;
            }
        }

        /* Advance to the next even candidate. */
        next_candidate += 2ULL;
    }

    return cache[target - 1];
}
