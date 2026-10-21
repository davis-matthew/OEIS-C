/*
 * A296023  Numbers n such that there is precisely 1 group of order n and 2 of
 *          order n + 1.
 * https://oeis.org/A296023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1663, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   has_two_groups_u64, is_cyclic_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296023: numbers n such that there is exactly 1 group of order n and
   2 of order n + 1.
   The code generates candidates by testing the conditions
   A000001(n) = 1  (cyclic)  and  A000001(n+1) = 2
   and caches the found terms so that subsequent calls are O(1). */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/has_two_groups_u64.h"
#include "../utils/is_cyclic_u64.h"

static seq_t *cached_terms = NULL;   /* found terms */
static size_t   cache_len   = 0;      /* number of stored terms   */
static size_t   cache_cap   = 0;      /* allocated capacity      */
static uint64_t last_checked = 0;     /* largest integer already tested */

/* ------------------------------------------------------------------ */
/*  Helper: grow the cached_terms array.                             */
/* ------------------------------------------------------------------ */
static void grow_cache(size_t new_cap)
{
    seq_t *tmp = (seq_t *)realloc(cached_terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failed  */
    cached_terms = tmp;
    cache_cap    = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Helper: test whether n (uint64_t) is cyclic (exactly one group). */

/* ------------------------------------------------------------------ */
/*  Helper: test whether n (uint64_t) has exactly two groups.        */

/* ------------------------------------------------------------------ */
/*  Ensure that at least 'need' terms have been generated.           */
/* ------------------------------------------------------------------ */
static void ensure_terms(idx_t need)
{
    while (cache_len < (size_t)need) {
        ++last_checked;                    /* next candidate */

        /* test candidate: A000001(last_checked) = 1  &&
                            A000001(last_checked+1) = 2 */
        uint64_t cand = last_checked;
        if (is_cyclic_u64(cand) &&
            has_two_groups_u64(cand + 1)) {

            /* store term */
            if (cache_len == cache_cap)
                grow_cache(cache_cap ? cache_cap * 2 : 4);

            cached_terms[cache_len++] = (seq_t)cand;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Main sequence function.                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                      /* undefined for n < 1   */
        return 0;

    ensure_terms(n);                  /* compute up to n-th term */
    return cached_terms[n - 1];       /* OEIS uses 1‑based offset */
}
