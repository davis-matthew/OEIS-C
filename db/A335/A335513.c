/*
 * A335513  Numbers k such that the k-th composition in standard order
 *          (A066099) avoids the pattern (1,1,1).
 * https://oeis.org/A335513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th index k such that the k‑th composition in
 *            A066099 (graded reverse‑lexicographic order) avoids the pattern
 *            (1,1,1)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"

/* Test whether the k‑th composition contains no part appearing
   more than twice.  The composition is obtained from the
   set of positions of 1's in the reversed binary expansion
   of k. */
static bool valid_k(idx_t k)
{
    /* A part size never exceeds 64 since k is at most 64 bits. */
    int counts[65] = {0};
    idx_t prev = 0;
    idx_t pos  = 1;

    while (k)
    {
        if (k & 1ULL)
        {
            unsigned part = (unsigned)(pos - prev);   /* part size */
            if (++counts[part] > 2)
                return false;
            prev = pos;
        }
        k >>= 1;
        ++pos;
    }
    return true;
}

/* Static storage for the filtered indices. */
static seq_t *valid_index = NULL;   /* array of qualifying k values (as seq_t) */
static size_t valid_cap   = 0;
static size_t valid_len   = 0;      /* number of stored values */
static idx_t   next_k      = 0;      /* next candidate k to test */

/* Grow the static array using the seq_realloc helper. */
static void grow_valid_array(void)
{
    size_t new_cap = (valid_cap == 0) ? 16 : valid_cap * 2;
    seq_t *new_arr = seq_realloc(valid_index, valid_cap, new_cap);
    if (!new_arr)
        abort();
    valid_index = new_arr;
    valid_cap   = new_cap;
}

/*  External linkage as required. */
extern seq_t a(idx_t n)
{
    /* Produce the first n qualifying indices. */
    while (valid_len < n)
    {
        if (valid_k(next_k))
        {
            if (valid_len == valid_cap)
                grow_valid_array();
            /* Convert the index to seq_t; fits for all requested n. */
            valid_index[valid_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return valid_index[n - 1];
}
