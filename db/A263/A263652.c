/*
 * A263652  Indices in A181391 (Van Eck's Sequence) where a new term first
 *          appears.
 * https://oeis.org/A263652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = index of the first occurrence of the n‑th new value
   in Van Eck’s sequence (OEIS A263652) */

/* The function simulates Van Eck’s sequence, storing the last and
   previous occurrence indices for each value. Whenever a new value
   appears it records its index in first_idx. The function works
   for arbitrary sequence element types (seq_t) and 64‑bit indices
   (idx_t). */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* dynamic arrays for last and previous occurrences of each value */
static idx_t *last_pos  = NULL;   /* index of last occurrence of v   */
static idx_t *prev_pos  = NULL;   /* index of previous occurrence of v */
static size_t val_cap   = 0;      /* capacity of the above arrays     */

/* array of first occurrence indices (1‑based) */
static idx_t *first_idx = NULL;   /* first_idx[k-1] = index of k‑th new value */
static size_t first_cap = 0;      /* capacity of first_idx */
static size_t first_len = 0;      /* number of first occurrences stored */

/* state while generating the Van Eck sequence */
static idx_t current_val   = 0;   /* value at current_index */
static idx_t current_index = 1;   /* 1‑based position */

/* forward declaration */
static void ensure_val_cap(idx_t needed);

/* grow the last_pos/prev_pos arrays to hold indices up to 'needed' */
static void ensure_val_cap(idx_t needed)
{
    if ((idx_t)val_cap > needed)
        return;

    size_t new_cap = val_cap ? val_cap : 1;
    while (new_cap <= (size_t)needed)
        new_cap <<= 1;

    last_pos = (idx_t *)realloc(last_pos, new_cap * sizeof(idx_t));
    if (!last_pos) abort();
    prev_pos = (idx_t *)realloc(prev_pos, new_cap * sizeof(idx_t));
    if (!prev_pos) abort();

    /* initialise new slots to zero */
    memset((void *)(last_pos + val_cap), 0,
           (new_cap - val_cap) * sizeof(idx_t));
    memset((void *)(prev_pos + val_cap), 0,
           (new_cap - val_cap) * sizeof(idx_t));

    val_cap = new_cap;
}

/* grow the first_idx array */
static void ensure_first_cap(size_t needed)
{
    if (first_cap >= needed)
        return;

    size_t new_cap = first_cap ? first_cap : 1;
    while (new_cap < needed)
        new_cap <<= 1;

    first_idx = (idx_t *)realloc(first_idx, new_cap * sizeof(idx_t));
    if (!first_idx) abort();
    first_cap = new_cap;
}

/* main function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined offset, placeholder */

    /* initialise static state on first call */
    if (!last_pos)
    {
        /* we need only value 0 initially */
        val_cap = 1;
        last_pos = (idx_t *)calloc(1, sizeof(idx_t));
        if (!last_pos) abort();
        prev_pos = (idx_t *)calloc(1, sizeof(idx_t));
        if (!prev_pos) abort();

        /* value 0 at index 1 */
        last_pos[0] = 1;   /* last occurrence of 0 */
        prev_pos[0] = 0;   /* no previous occurrence before index 1 */

        /* first occurrence indices array */
        first_cap = 1;
        first_idx = (idx_t *)calloc(1, sizeof(idx_t));
        if (!first_idx) abort();
        first_idx[0] = 1;             /* index 1 is the first new value */
        first_len = 1;

        current_val   = 0;
        current_index = 1;
    }

    /* generate Van Eck terms until we have at least n first occurrences */
    while (first_len < (size_t)n)
    {
        idx_t next_index = current_index + 1;

        /* compute next value of Van Eck’s sequence */
        idx_t next_val;
        if (prev_pos[current_val] == 0)
            next_val = 0;
        else
            next_val = current_index - prev_pos[current_val];

        /* ensure arrays large enough for next_val */
        ensure_val_cap(next_val + 1);

        /* check whether next_val is new */
        if (last_pos[next_val] == 0)
        {
            /* record first occurrence */
            ensure_first_cap(first_len + 1);
            first_idx[first_len++] = next_index;
        }

        /* update occurrence information for next_val */
        idx_t old_last = last_pos[next_val];
        prev_pos[next_val] = old_last;
        last_pos[next_val] = next_index;

        /* advance to next position */
        current_val   = next_val;
        current_index = next_index;
    }

    /* return the n‑th first occurrence index */
    return (seq_t)first_idx[n - 1];
}
