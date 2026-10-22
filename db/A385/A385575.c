/*
 * A385575  Numbers whose binary indices have the same number of adjacent
 *          parts differing by 1 as adjacent parts differing by more than 1.
 * https://oeis.org/A385575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generate terms of A385575: numbers whose binary indices have the same
 * number of adjacent parts differing by 1 as adjacent parts differing by
 * more than 1.  The function performs a sequential search and caches
 * the found terms. */

#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Helper: test whether a number has the required property.             */
/* --------------------------------------------------------------------- */
static bool good_number(idx_t v)
{
    idx_t x = v;
    int prev = 0;           /* position (1‑based) of previous 1‑bit */
    int adj  = 0;           /* differences == 1  */
    int non  = 0;           /* differences > 1  */
    int pos  = 0;           /* current bit index (0‑based) */

    while (x != (idx_t)0) {
        if ((x & (idx_t)1) != 0) {
            int cur = pos + 1;               /* 1‑based position */
            if (prev != 0) {
                int diff = cur - prev;
                if (diff == 1)
                    ++adj;
                else
                    ++non;
            }
            prev = cur;
        }
        x >>= 1;
        ++pos;
    }
    return adj == non;
}

/* --------------------------------------------------------------------- */
/* Cache storage for computed sequence terms.                            */
/* --------------------------------------------------------------------- */
static seq_t *cache          = NULL;
static size_t cache_len      = 0;   /* number of cached terms */
static size_t cache_cap      = 0;   /* allocated capacity */
static idx_t next_candidate  = 1;   /* next integer to test */

/* Allocate or resize the cache array. */
static void realloc_cache(size_t new_cap)
{
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (cache == NULL) {
        /* In an OEIS environment exit on allocation failure. */
        exit(EXIT_FAILURE);
    }
    cache_cap = new_cap;
}

/* Ensure that at least `need` terms are cached. */
static void ensure_cache(idx_t need)
{
    if (cache_len >= (size_t)need) {
        return;
    }

    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) {
            exit(EXIT_FAILURE);
        }
    }

    while (cache_len < (size_t)need) {
        /* Find the next integer satisfying the property. */
        while (!good_number(next_candidate)) {
            ++next_candidate;
        }

        /* Grow the cache if needed. */
        if (cache_len == cache_cap) {
            realloc_cache(cache_cap * 2);
        }

        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* --------------------------------------------------------------------- */
/* Sequence entry function.                                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        /* Undefined below the offset; return 0 as a safe placeholder. */
        return (seq_t)0;
    }

    ensure_cache(n);
    return cache[n - 1];   /* sequence is 1‑based */
}
