/*
 * A389713  a(1) = 3, a(2) = 5, a(n) is the smallest prime such that a(n) -
 *          a(n-1) + 1 is in the sequence.
 * https://oeis.org/A389713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 3974, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3, a(2)=5, a(n) = smallest prime p such that
   p - a(n-1) + 1 is an earlier term of the sequence. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"

/* Static memoised cache of computed terms */
static seq_t *cache      = NULL;
static size_t cache_len  = 0; /* number of terms cached */
static size_t cache_cap  = 0; /* allocated capacity */

/* Ensure that the cache has space for at least 'needed' terms. */
static void ensure_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 2;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Binary search: true if 'val' exists in the first 'len' elements of 'arr'. */
static bool contains(const seq_t *arr, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] == val) return true;
        if (arr[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* out of range; not used */

    /* Initialise cache with the two known terms on first call. */
    if (cache_len == 0) {
        ensure_cap(2);
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)5;
        cache_len = 2;
    }

    /* If already computed, return it. */
    if ((size_t)n <= cache_len) return cache[n - 1];

    /* Extend cache up to the requested index. */
    for (size_t idx = cache_len + 1; idx <= (size_t)n; ++idx) {
        /* Previous term in the sequence. */
        seq_t prev = cache[cache_len - 1];

        /* Start searching from the next prime after 'prev'. */
        uint64_t cand = next_prime((uint64_t)prev + 1);

        /* Find the smallest prime satisfying the recurrence. */
        while (1) {
            seq_t v = (seq_t)cand - prev + 1;   /* potential related value */
            if (contains(cache, cache_len, v)) {
                ensure_cap(cache_len + 1);
                cache[cache_len] = (seq_t)cand;
                ++cache_len;
                break;
            }
            cand = next_prime(cand);   /* try the next prime */
        }
    }

    return cache[n - 1];
}
