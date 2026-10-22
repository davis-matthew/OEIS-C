/*
 * A321523  List of pairs: primes whose reversal is also prime, each followed
 *          by its reversal.
 * https://oeis.org/A321523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) = the n-th element of the list of all primes p where rev(p)
 *            is prime, followed by rev(p). The sequence is 1‑based
 * Range:     u8 n <= 28, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   alloc_seq_array, is_prime_big, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Utilities provided by the build system. */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n);      */
#include "../utils/rev.h"            /* seq_t rev(idx_t x);             */
#include "../utils/alloc_seq_array.h"/* seq_t *alloc_seq_array(size_t);  */

static void grow_cache(seq_t **cache, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = *cap ? *cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = alloc_seq_array(new_cap);
    if (!tmp) abort();
    *cache = tmp;
    *cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1; safe default */

    /* Static cache of sequence terms, filled lazily. */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0, cache_cap = 0;
    static uint64_t candidate = 0;  /* current candidate to test */

    /* Ensure enough space in the cache. */
    grow_cache(&cache, &cache_cap, (size_t)n);

    /* Generate terms until we have one at index n. */
    while (cache_sz < (size_t)n)
    {
        /* Find next prime candidate. */
        if (candidate == 0) candidate = 2;
        while (!is_prime_big((seq_t)candidate))
        {
            candidate += (candidate == 2 ? 1 : 2);  /* advance to next odd */
        }

        /* Check whether the reversal is also prime. */
        seq_t rev_candidate = rev(candidate);
        if (is_prime_big(rev_candidate))
        {
            /* Append the pair to the cache. */
            grow_cache(&cache, &cache_cap, cache_sz + 2);
            cache[cache_sz++] = (seq_t)candidate;
            cache[cache_sz++] = rev_candidate;
        }

        /* Advance to the next candidate for the next loop iteration. */
        candidate += (candidate == 2 ? 1 : 2);
    }

    /* Return the 1‑based term. */
    return cache[n - 1];
}
