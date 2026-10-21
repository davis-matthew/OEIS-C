/*
 * A249724  Numbers k such that on row k of Pascal's triangle there is no
 *          multiple of 9 which would be less than any (potential) multiple of
 *          4 on the same row.
 * https://oeis.org/A249724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 177, u16 n <= 9815, u32 n <= 9815, u64 n <= 9815,
 *            u128 n <= 9815, big any n
 * Helpers:   include_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A249724
    Numbers k such that on row k of Pascal's triangle there is no multiple
    of 9 which would be less than any (potential) multiple of 4 on the same
    row.  The function `a(n)` returns the n‑th such k (1‑based index). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/include_row.h"

/*  a(n) – the n‑th term of OEIS A249724 (1‑based index). */
seq_t a(idx_t n)
{
    /* Static state for lazy generation. */
    static seq_t *terms = NULL;   /* array of previously found k's */
    static size_t terms_len = 0;  /* number of entries stored */
    static size_t terms_cap = 0;  /* allocated capacity */
    static idx_t next_k = 0;      /* next row index to examine */

    /* Ensure we have at least n terms. */
    while (terms_len < n)
    {
        /* Allocate (or grow) storage if needed before using it. */
        if (terms_len >= terms_cap)
        {
            size_t new_cap = terms_cap ? terms_cap * 2 : 1024;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp)      /* allocation failure: abort gracefully */
                return (seq_t)0;
            terms = tmp;
            terms_cap = new_cap;
        }

        if (include_row(next_k))
            terms[terms_len++] = (seq_t)next_k;

        ++next_k;
    }

    return terms[n - 1];
}
