/*
 * A303113  Number of partitions of n into two distinct positive integers
 *          (s,t), s < t, such that t is semiprime and |t-s| is not semiprime.
 * https://oeis.org/A303113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A303113
 * Formula: a(n) = Sum_{i=1..floor((n-1)/2)} [Omega(n-i) = 2] * (1-[Omega(n-2*i) = 2]) 
 * where Omega(n) is the big-omega function (number of prime factors with multiplicity).
 * The code precomputes semiprime status up to the largest needed argument using
 * bigomega() from the utils library. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/bigomega.h"

/* Cache to store whether a number up to a certain limit is a semiprime.
 * semiprime_cache[n] == 1 iff Omega(n) == 2 (and n > 1). */
static unsigned char *semiprime_cache = NULL;
static idx_t semiprime_limit = 0;

/* Ensure semiprime_cache contains data up to 'limit' (inclusive). */
static void ensure_semiprime_limit(idx_t limit)
{
    if (limit <= semiprime_limit)
        return;

    /* Reallocate to new size */
    unsigned char *new_cache =
        (unsigned char*)realloc(semiprime_cache,
                                (size_t)(limit + 1) * sizeof(unsigned char));
    if (!new_cache)
        exit(EXIT_FAILURE);    /* memory exhaustion */

    /* Fill in cache entries from old limit+1 to new limit */
    for (idx_t n = semiprime_limit + 1; n <= limit; ++n)
    {
        if (n < 2)
            new_cache[n] = 0;
        else
        {
            seq_t omega = bigomega(n);   /* Omega(n) */
            new_cache[n] = (omega == (seq_t)2) ? 1 : 0;
        }
    }

    semiprime_cache = new_cache;
    semiprime_limit = limit;
}

/* Test whether n (<= semiprime_limit) is a semiprime. */
static bool is_semiprime(idx_t n)
{
    return n <= semiprime_limit && semiprime_cache[n];
}

/* Sequence entry a(n). */
seq_t a(idx_t n)
{
    seq_t result = 0;

    /* We need to know semiprime status for numbers up to n‑1
     * (t) and up to n‑2 (|t‑s|).  Ensuring up to n‑1 suffices. */
    if (n > 1)
        ensure_semiprime_limit(n - 1);

    /* Enumerate pairs (s, t) with s < t, s + t = n.  t > n/2. */
    idx_t half = n / 2;
    for (idx_t t = half + 1; t < n; ++t)
    {
        if (!is_semiprime(t))
            continue;

        idx_t d = 2 * t - n;   /* d = |t - s| = 2*t - n > 0 */
        if (is_semiprime(d))
            continue;

        ++result;
    }
    return result;
}
