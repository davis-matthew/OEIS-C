/*
 * A336909  Numbers k > 2 such that omega(k) > log(log(k)) +
 *          sqrt(log(log(k))), where omega(k) is the number of distinct primes
 *          dividing k (A001221).
 * https://oeis.org/A336909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A336909: numbers k > 2 such that omega(k) > log(log(k)) + sqrt(log(log(k))) */
/* Computed by a linear scan with caching.                                      */
#include <stddef.h>              /* size_t, NULL */
#include <stdlib.h>              /* malloc, realloc, free */
#include <stdint.h>              /* uint64_t */
#include <stdbool.h>             /* bool, true, false */
#include <math.h>                /* log, sqrt */
#include "../utils/count_distinct_primes.h"  /* int count_distinct_primes(uint64_t n); */

/* Append a value to a dynamic array, expanding its capacity as needed.       */
static void append_candidate(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len >= *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* unrecoverable allocation failure */
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

/* Main function implementing sequence A336909.                                 */
seq_t a(idx_t n)
{
    /* Static cache: discovered terms, its length, capacity, and next integer to test. */
    static seq_t *cache  = NULL;   /* dynamic array of qualifying numbers */
    static size_t  len   = 0;      /* number of elements stored in cache */
    static size_t  cap   = 0;      /* allocated capacity of cache */
    static idx_t   cur   = 3;      /* next integer to test (k > 2) */

    if (n == 0) return 0;          /* offset is 1; guard against zero */

    /* Extend the cache until we have at least n terms. */
    while (len < n) {
        uint64_t k = (uint64_t)cur;            /* candidate integer */
        int omega = count_distinct_primes(k);  /* distinct prime factor count */

        /* Compute threshold: log(log(k)) + sqrt(log(log(k))) using double precision. */
        double L = log(log((double)k));
        double thr = L + sqrt(L);

        if ((double)omega > thr) {
            /* Store qualifying number, converted to seq_t.         */
            append_candidate(&cache, &len, &cap, (seq_t)cur);
        }
        cur++;                                /* move to next integer */
    }

    return cache[n - 1];    /* 1‑based index to 0‑based array offset */
}
