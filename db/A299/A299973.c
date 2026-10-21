/*
 * A299973  Lexicographic first sequence of positive integers such that a(n) +
 *          a(n+1) has a digit 3, and no term occurs twice.
 * https://oeis.org/A299973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   has_digit_three
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A299973: lexicographically first permutation of positive integers
   such that a(n) + a(n+1) contains a digit 3 and all terms are distinct.
   The value is produced by a greedy search. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_digit_three.h"

/* Dynamic arrays for the sequence and the set of used numbers. */
static seq_t *cache      = NULL; /* computed terms */
static size_t cache_len  = 0;    /* number of terms stored */
static size_t cache_cap  = 0;    /* allocated capacity of cache */

static seq_t *used      = NULL; /* used numbers, kept sorted */
static size_t used_len  = 0;    /* number of used values stored */
static size_t used_cap  = 0;    /* allocated capacity of used */

/* Grow the cache array to at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    if (cache_cap == 0) cache_cap = 16;
    while (cache_cap < new_cap) cache_cap *= 2;
    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
}

/* Grow the used array to at least new_cap elements. */
static void grow_used(size_t new_cap)
{
    if (new_cap <= used_cap) return;
    if (used_cap == 0) used_cap = 16;
    while (used_cap < new_cap) used_cap *= 2;
    used = (seq_t *)realloc(used, used_cap * sizeof(seq_t));
}

/* Binary search for val in the sorted used array.
   Returns its index if found, or -1 if not present. */
static int used_find(seq_t val)
{
    size_t lo = 0, hi = used_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (used[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < used_len && used[lo] == val)
        return (int)lo;
    return -1;
}

/* Insert val into the sorted used array, keeping the order. */
static void used_insert(seq_t val)
{
    /* Find insertion position. */
    size_t pos = 0;
    size_t lo = 0, hi = used_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (used[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    pos = lo;

    grow_used(used_len + 1);

    /* Shift elements to make room. */
    for (size_t i = used_len; i > pos; --i)
        used[i] = used[i - 1];

    used[pos] = val;
    ++used_len;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined below offset, harmless */

    /* Generate terms up to index n (1‑based). */
    while (cache_len < (size_t)n) {
        size_t idx = cache_len;                 /* 0‑based index */

        if (idx == 0) {                         /* a(1) = 1 */
            grow_cache(1);
            grow_used(1);
            cache[0] = (seq_t)1;
            used[0]  = (seq_t)1;
            ++cache_len;
            ++used_len;
            continue;
        }

        seq_t prev = cache[cache_len - 1];
        seq_t cand = (seq_t)1;

        /* Find the smallest unused candidate such that prev + cand
           contains the digit 3. */
        while (true) {
            /* Skip used numbers. */
            if (used_find(cand) != -1) {
                ++cand;
                continue;
            }

            seq_t sum = prev + cand;
            if (has_digit_three(sum))
                break;          /* suitable candidate found */

            ++cand;
        }

        /* Append the new term. */
        grow_cache(cache_len + 1);
        cache[cache_len++] = cand;
        used_insert(cand);
    }

    return cache[(size_t)n - 1];
}
