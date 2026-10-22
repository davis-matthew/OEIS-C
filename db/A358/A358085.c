/*
 * A358085  Inventory of positions ordered by binary lengths of terms, as an
 *          irregular table; the first row contains 1, subsequent rows
 *          contains the 1-based positions of terms with binary length 1,
 *          followed by positions of terms with binary length 2, 3, etc. in
 *          prior rows flattened.
 * https://oeis.org/A358085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 511, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alg. description: build rows where each new row contains the 1‑based
   positions of the current sequence entries ordered by binary length
   (grouping by length, keeping the internal order).  This produces
   OEIS A358085.  The sequence is generated on demand and memoised
   in static storage. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* Static state */
static seq_t *A   = NULL;     /* 1‑based sequence (A[0] dummy) */
static idx_t  len = 0;        /* number of terms stored (excluding dummy) */
static size_t cap = 0;        /* capacity of A array */

/* Ensure that A has at least n terms.  (This function is thread‑safe
   only with external synchronisation; the required build environment
   does not use threading.) */
static void grow_array(idx_t needed)
{
    size_t new_cap = cap;
    if (new_cap == 0) new_cap = 4;
    while ((size_t)needed + 1 > new_cap) new_cap <<= 1;
    if (new_cap > cap) {
        seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        A   = tmp;
        cap = new_cap;
    }
}

static void ensure_up_to(idx_t n)
{
    if (n <= len) return;

    if (len == 0) {
        /* initialise first row: [1] */
        grow_array(1);
        A[0] = (seq_t)0;   /* dummy to keep 1‑based indexing */
        A[1] = (seq_t)1;
        len  = 1;
    }

    /* iterate until we have at least n terms */
    while (len < n) {
        idx_t cur_len = len;                /* current sequence length */

        /* counting sort on binary lengths */
        unsigned counts[129] = {0};         /* max bitlen assumed < 129 */
        unsigned max_len     = 0;
        for (idx_t i = 1; i <= cur_len; ++i) {
            unsigned l = bitlen_seq_t(A[i]);    /* binary length of A[i] */
            if (l > max_len) max_len = l;
            ++counts[l];
        }

        unsigned start[129] = {0};
        unsigned next [129] = {0};

        /* cumulative start indices for each length */
        start[1] = 0;
        for (unsigned l = 1; l <= max_len; ++l) {
            start[l + 1] = start[l] + counts[l];
        }
        for (unsigned l = 1; l <= max_len; ++l) {
            next[l] = start[l];
        }

        /* temporary array to hold the new row (positions of indices) */
        idx_t *row = (idx_t *)malloc(cur_len * sizeof(idx_t));
        if (!row) exit(1);

        for (idx_t i = 1; i <= cur_len; ++i) {
            unsigned l = bitlen_seq_t(A[i]);      /* length of term A[i] */
            unsigned pos = next[l]++;             /* target slot in row */
            row[pos] = (idx_t)i;                  /* store the position */
        }

        /* append the new row to the sequence storage */
        grow_array(len + cur_len);
        for (size_t k = 0; k < (size_t)cur_len; ++k) {
            A[len + 1 + k] = (seq_t)row[k];
        }
        len += cur_len;

        free(row);
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* safety for out‑of‑range input */
    ensure_up_to(n);
    return A[n];
}
