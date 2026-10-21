/*
 * A277144  Lexicographically least sequence of nonnegative integers that
 *          avoids 5/4-powers.
 * https://oeis.org/A277144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lexicographically least integer avoiding 5/4-powers *
 *            (factor xyx with |y| = 3|x| ≥ 1); constructed incrementally by *
 *            choosing the smallest candidate not creating such a pattern
 * Range:     u8 n <= 9843, u16 n <= 9843, u32 n <= 9843, u64 n <= 9843,
 *            u128 n <= 9843, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache for already computed terms */
static seq_t *cache = NULL;      /* array of seq_t values */
static size_t cache_cap = 0;     /* capacity of the cache array */
static idx_t cache_len = 0;      /* number of terms computed (last index + 1) */

/* Ensure that indices up to n are present in the cache. */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;

    /* Grow the cache if necessary. */
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!tmp) abort();     /* allocation failure */
        if (cache) {
            for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
            free(cache);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute the terms from cache_len up to n. */
    for (idx_t pos = cache_len; pos <= n; ++pos) {
        seq_t cand = 0;
        while (true) {
            bool bad = false;

            /* Check every possible x with 5*x <= pos+1. */
            for (idx_t x = 1; 5 * x <= pos + 1; ++x) {
                idx_t start1 = pos - 5 * x + 1;    /* first x block start */
                idx_t start2 = pos - x + 1;        /* last x block start */

                bool mismatch = false;
                for (idx_t k = 0; k < x; ++k) {
                    seq_t val1 = cache[(size_t)start1 + k];
                    seq_t val2;
                    if (start2 + k == pos)
                        val2 = cand;                /* new value in the last block */
                    else
                        val2 = cache[(size_t)start2 + k];

                    if (val1 != val2) { mismatch = true; break; }
                }

                if (!mismatch) { /* pattern found; candidate bad */
                    bad = true;
                    break;
                }
            }

            if (!bad) break;   /* candidate OK */
            ++cand;            /* try the next integer */
        }
        cache[(size_t)pos] = cand;
    }

    cache_len = (idx_t)needed;
}

/* Public sequence routine: returns a(n) for any n ≥ 0. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
