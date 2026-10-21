/*
 * A282430  Even numbers not divisible by 3 which are not of the form p + 3^x
 *          with p prime.
 * https://oeis.org/A282430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 693, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS definition:
   a(n) = the n-th even integer not divisible by 3 that cannot be expressed
   as p + 3^x with p prime.  The function builds the sequence on demand
   and memoises results for fast repeated queries.  Search is performed
   with a simple loop over candidates, testing each 3‑power difference. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Return true if n can be written as p + 3^x with p prime. */
static bool has_prime_diff(seq_t n)
{
    for (seq_t pw = 1; pw < n; ) {
        seq_t d = n - pw;
        if (d >= 2 && is_prime((idx_t)d))
            return true;                 /* found a prime representation */
        /* Break when the next power of 3 would overflow or exceed n. */
        if (pw > n / 3) break;
        pw *= 3;
    }
    return false;
}

/* External function required by the specification. */
seq_t a(idx_t n)
{
    /* Empty cache for the first call. */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = 0;

    if (n < 1) return 0;   /* undefined for n < 1, but return 0 */

    /* Build the cache up to the requested index. */
    while (cache_len < n) {
        /* Find the next candidate: next even number not multiple of 3. */
        seq_t cand = last_checked + 2;
        for (;; cand += 2) {
            if (cand % 3 == 0) continue;
            if (!has_prime_diff(cand)) break;   /* candidate found */
        }

        /* Grow the cache array if necessary. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
#ifdef __cplusplus
            seq_t *tmp = new seq_t[new_cap];
            for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
            delete[] cache;
            cache = tmp;
#else
            cache = realloc(cache, new_cap * sizeof(seq_t));
#endif
            cache_cap = new_cap;
        }

        cache[cache_len++] = cand;
        last_checked = cand;
    }

    return cache[n - 1];
}
