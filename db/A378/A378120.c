/*
 * A378120  a(n) = (A000217(n) + A005132(n))/2.
 * https://oeis.org/A378120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n*(n+1)/2 + Recaman(n)) / 2
 * Range:     u8 n <= 22, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Cache for Recaman values up to the largest requested index */
static seq_t *rec_cache = NULL;   /* Recaman numbers a(0)…a(rec_len-1) */
static size_t  rec_len  = 0;      /* number of stored values          */
static size_t  rec_cap  = 0;      /* capacity of rec_cache            */

/* Check whether a value already occurs in the Recaman cache */
static bool rec_seen(seq_t val)
{
    for (size_t i = 0; i < rec_len; ++i)
        if (rec_cache[i] == val)
            return true;
    return false;
}

/* Compute Recaman numbers up to and including index n */
/* This routine may be called with decreasing n; it keeps the cache at the maximum size. */
static void recaman_to(idx_t n)
{
    /* Lazily allocate storage for the first call */
    if (rec_cache == NULL) {
        rec_cap = (size_t)n + 1;
        rec_cache = (seq_t *)malloc(rec_cap * sizeof(seq_t));
        if (!rec_cache) return;          /* allocation failure: give up */
        rec_cache[0] = 0;
        rec_len = 1;
    }

    /* Nothing to do if we already have enough terms */
    if (rec_len - 1 >= n)
        return;

    /* Grow the cache if necessary */
    if (rec_cap <= (size_t)n) {
        size_t new_cap = rec_cap << 1;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(rec_cache, new_cap * sizeof(seq_t));
        if (tmp) {
            rec_cache = tmp;
            rec_cap = new_cap;
        }
        /* On realloc failure we keep the old capacity; the loop below will add terms later */
    }

    /* Generate missing Recaman values */
    for (idx_t k = rec_len; k <= n; ++k) {
        seq_t prev = rec_cache[rec_len - 1];
        seq_t next;
        if (prev > (seq_t)k) {
            seq_t diff = prev - (seq_t)k;
            if (!rec_seen(diff))
                next = diff;
            else
                next = prev + (seq_t)k;
        } else {
            next = prev + (seq_t)k;
        }
        rec_cache[rec_len++] = next;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* a(0) = 0 */

    recaman_to(n);

    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;   /* triangular number A000217(n) */
    seq_t rec = rec_cache[n];                    /* Recaman number A005132(n) */
    return (tri + rec) / 2;
}
