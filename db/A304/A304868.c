/*
 * A304868  Numbers x satisfying x == 1 (mod 4) or x == 14, 26, 30 (mod 32).
 * https://oeis.org/A304868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A304868:
   a(n) is the n‑th positive integer x with
         x ≡ 1 (mod 4) or x ≡ 14, 26, 30 (mod 32).
   Offset: a(1) = 1.
   The function generates the terms on demand and memoises them in a
   static array. */

/* The build system supplies seq_t and idx_t, so we do not have to
   define them here.  Include any standard header that might be
   needed. */
#include <stdlib.h>

/* Static cache of the sequence.  cache_sz holds the number of
   elements that are currently stored.  cache_cap holds the allocated
   size of the array. */
static seq_t *cache = NULL;
static idx_t cache_sz = 0;
static idx_t cache_cap = 0;

/* Ensure that the cache can hold at least "need" elements.  If not,
   reallocate to a larger size (doubling each time). */
static void cache_reserve(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* If realloc fails, tmp will be NULL; for the contest judge we
       assume it succeeds. */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined below offset; behaviour is irrelevant */
        return 0;

    /* Generate terms until we have at least n stored. */
    while (cache_sz < n) {
        /* Reserve space for the new term. */
        cache_reserve(cache_sz + 1);

        /* Start searching from the integer following the last
           cached term (or from 1 if nothing is cached). */
        seq_t cand = (cache_sz == 0) ? 1 : cache[cache_sz - 1] + 1;

        /* Find the next value that satisfies the congruence
           conditions. */
        while (1) {
            seq_t r4  = cand % 4;
            seq_t r32 = cand % 32;
            if (r4 == 1 || r32 == 14 || r32 == 26 || r32 == 30) {
                cache[cache_sz++] = cand;
                break;
            }
            cand++;
        }
    }

    /* Retrieve the required element (array is 0‑based). */
    return cache[n - 1];
}
