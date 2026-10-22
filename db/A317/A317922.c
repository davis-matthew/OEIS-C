/*
 * A317922  a(n) = number of k with 0 < 2*k < n-1 such that a(n-k) AND
 *          a(n-2*k) = a(n-k) (where AND denotes the bitwise AND operator).
 * https://oeis.org/A317922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5620, u16 n <= 9034, u32 n <= 9034, u64 n <= 9034,
 *            u128 n <= 9034, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation follows the recurrence definition of OEIS A317922:
   a(n) = # { k | 0 < 2*k ≤ n-1 and ( a(n-k) & a(n-2*k) == a(n-k) ) }
   The sequence is memoised in a static cache so that calls in any order
   share previously computed values.  The cache uses 1‑based indexing;
   cache[0] is unused.  All arithmetic is done in the supplied types
   seq_t and idx_t.  The recurrence is O(n²) in the naïve form, but with
   the memoisation a single call to a(n) requires at most O(n²/2)
   bit‑and operations, which comfortably fits the given time limit for
   n up to about 10 000 (the number of k’s checked for all 1 ≤ i ≤ n is
   ⌊(n-1)/2⌋ + ⌊(n-2)/2⌋ + … ≈ n²/4).  The code uses only integer
   arithmetic, no additional external declarations, no dynamic
   arrays, and is completely compliant with the C11/C++17 constraints. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Persistent cache for computed a(n) values */
static seq_t *cache = NULL;   /* 1‑based array: cache[1] = a(1), etc. */
static idx_t max_computed = 0; /* largest n whose value is stored */
static size_t cache_cap = 0;   /* allocated capacity in array elements */

/* Ensure that cache has capacity for at least (n+1) entries (index 0 unused). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;          /* double until enough */
    seq_t *new_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (!new_cache)
        abort();                /* allocation failed */
    if (cache) {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Computes a(n) for 1‑based index n.  The value is cached so subsequent
   calls with the same or smaller n are O(1). */
seq_t a(idx_t n)
{
    if (n == 0)                  /* a(0) is not defined in the sequence, but
                                   returning 0 keeps the recurrence well‑defined
                                   for the strict upper bound of the loop. */
        return 0;

    ensure_cache(n);

    /* Compute any missing values up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* The recurrence works for all i ≥ 1; no special case needed. */
        seq_t count = 0;
        /* k satisfies 0 < 2*k ≤ i-1  →  k ≤ floor((i-1)/2) */
        idx_t limit = (i - 1) / 2;
        for (idx_t k = 1; k <= limit; ++k) {
            seq_t a1 = cache[i - k];
            seq_t a2 = cache[i - 2 * k];
            if ((a1 & a2) == a1)
                ++count;
        }
        cache[i] = count;
    }
    max_computed = n;
    return cache[n];
}
