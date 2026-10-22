/*
 * A329221  a(0)=0. If a(n)=k is the first occurrence of k then a(n+1)=a(k),
 *          otherwise a(n+1)=n-m where m is the index of the greatest prior
 *          term.
 * https://oeis.org/A329221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0. For n≥1, let k=a(n−1).  
   If k has not occurred before, a(n)=a(k);  
   otherwise a(n)=(n−1)−m, where m is the index of the greatest prior term
   among a(0)…a(n−2).  All indices are 0‑based. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

/* static cache for memoisation */
static seq_t *cache = NULL;   /* array of computed terms */
static size_t cap   = 0;      /* allocated capacity */
static size_t len   = 0;      /* number of terms stored */

/* Ensure storage for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    idx_t needed = n + 1;
    if (cap >= needed) return;
    size_t new_cap = (cap == 0) ? 64 : cap;
    while (new_cap < (size_t)needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cap   = new_cap;
}

/* Return the sequence value a(n) for any n ≥ 0. */
seq_t a(idx_t n)
{
    /* grow cache to contain up to n */
    ensure_capacity(n);

    /* initialise first term if necessary */
    if (len == 0) {
        cache[0] = (seq_t)0;
        len = 1;
    }

    /* if already computed, return */
    if ((idx_t)len > n) return cache[n];

    /* compute terms from len up to n */
    for (idx_t i = len; i <= n; ++i) {
        idx_t prev = i - 1;                     /* n-1 in the recurrence */
        seq_t last = cache[prev];               /* k = a(n-1) */

        /* check if this is the first occurrence of 'last' among indices < prev */
        bool first = true;
        for (idx_t j = 0; j < prev; ++j) {
            if (cache[j] == last) { first = false; break; }
        }

        seq_t val;
        if (first) {
            /* first occurrence: a(n) = a(k) where k = last */
            idx_t k = (idx_t)last;
            /* by definition k < prev, so a(k) is already computed */
            val = cache[k];
        } else {
            /* not first: a(n) = (n-1) - m, where m is index of greatest prior term */
            idx_t max_idx = 0;
            seq_t max_val = cache[0];
            for (idx_t j = 1; j < prev; ++j) {
                /* for ties, keep the later index (greatest prior term) */
                if (cache[j] >= max_val) {
                    max_val = cache[j];
                    max_idx = j;
                }
            }
            val = (seq_t)(prev - max_idx);
        }

        cache[i] = val;
    }

    len = n + 1;      /* now cache has terms up to index n */
    return cache[n];
}
