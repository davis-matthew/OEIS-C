/*
 * A335480  Numbers k such that the k-th composition in standard order
 *          (A066099) matches the pattern (1,3,2).
 * https://oeis.org/A335480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A335480: k such that the k‑th composition in standard order
   (graded reverse‑lexicographic, A066099) contains a subsequence of three
   parts whose relative order is (1,3,2) – i.e. the first part is the
   smallest, the second part is the largest, and the third part is the
   middle.  The function works for any build: seq_t may be unsigned 8–128
   bits or an arbitrary‑precision integer.  The value of a(n) is the
   n‑th k≥1 that satisfies the pattern. */

#include <stdlib.h>
#include <stdbool.h>

/* Test whether the composition of k (k >= 1) contains a (1,3,2) pattern. */
static bool matches_pattern(idx_t k)
{
    /* Get positions of 1's in the reversed binary expansion of k. */
    idx_t pos[64];
    size_t plen = 0;
    idx_t prev = 0;
    idx_t pos_counter = 1;           /* positions start at 1 for LSB */
    idx_t temp = k;

    while (temp)
    {
        if (temp & 1U)
        {
            pos[plen++] = pos_counter - prev; /* first differences */
            prev = pos_counter;
        }
        temp >>= 1;
        pos_counter++;
    }

    if (plen < 3)                     /* fewer than three parts */
        return false;

    /* Compose the parts in the natural order: reverse the pos array. */
    idx_t comp[64];
    for (size_t i = 0; i < plen; ++i)
        comp[i] = pos[plen - 1 - i];

    /* Search for indices i < j < k with comp[i] < comp[k] < comp[j]. */
    for (size_t i = 0; i + 2 < plen; ++i)
    {
        for (size_t j = i + 1; j + 1 < plen; ++j)
        {
            for (size_t k2 = j + 1; k2 < plen; ++k2)
            {
                if (comp[i] < comp[k2] && comp[k2] < comp[j])
                    return true;
            }
        }
    }
    return false;
}

/* External function required by the build system.  a(1) = 50. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0, cache_cap = 0;
    static idx_t next_k = 1;          /* start scanning from 1 */

    if (n == 0)
        return (seq_t)0;              /* undefined, but harmless */

    /* Extend the cache until it contains at least n entries. */
    while (cache_size < (size_t)n)
    {
        /* Find the next k that matches the pattern. */
        while (true)
        {
            if (matches_pattern(next_k))
            {
                /* Grow the cache if necessary. */
                if (cache_size == cache_cap)
                {
                    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();          /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = (seq_t)next_k;
                ++next_k;                      /* move to next candidate */
                break;
            }
            ++next_k;
        }
    }
    return cache[n - 1];
}
