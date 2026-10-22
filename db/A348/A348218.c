/*
 * A348218  Variation on the Inventory Sequence A342585: record the number of
 *          previous terms which are divisible by the iterating number,
 *          starting at 1, until 0 is recorded, then restart the iterating
 *          number from 1. See the Comments.
 * https://oeis.org/A348218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A348218: variation on the inventory sequence.
   Recurrence:
     a(0) = 1;
     For n > 0 let d be the current divisor.
     a(n) = number of previous terms a(k) (k < n) that are divisible by d.
     After computing a(n), if a(n) == 0 then d = 1, else d = d + 1.
   The implementation memoises the sequence up to the largest n requested.
*/

#include <stdlib.h>
#if defined(__cplusplus)
#  include <new>          /* for placement new */
#endif

/* Storage of memoised terms and state */
static seq_t *seq = NULL;          /* array of sequence values */
static idx_t sz   = 0;             /* number of stored terms */
static idx_t ddiv = 1;             /* divisor for the next term to be generated */

/* Allocate or grow the sequence array */
static void ensure_capacity(idx_t new_sz) {
    if (new_sz <= sz) return;
    /* grow to new_sz elements */
    seq_t *tmp = (seq_t *)realloc(seq, sizeof(seq_t) * (size_t)new_sz);
    if (!tmp) /* abort on allocation failure */ abort();
    seq = tmp;
#if defined(__cplusplus)
    /* construct any new elements that may contain non‑trivial constructors */
    for (idx_t i = sz; i < new_sz; ++i)
        ::new ((void *)&seq[i]) seq_t();
#endif
    sz = new_sz;
}

seq_t a(idx_t n) {
    /* initialise first term if not yet done */
    if (sz == 0) {
        ensure_capacity(1);
        seq[0] = (seq_t)1;
        ddiv = 1;          /* divisor for the next term a(1) */
    }

    /* extend memoisation to include a(n) */
    if (n >= sz) {
        for (idx_t i = sz; i <= n; ++i) {
            seq_t count = (seq_t)0;
            /* count previous terms that are >0 and divisible by ddiv */
            for (idx_t j = 0; j < i; ++j) {
                if (seq[j] != (seq_t)0 && seq[j] % (seq_t)ddiv == (seq_t)0)
                    ++count;
            }
            ensure_capacity(i + 1);          /* ensure space for new term */
            seq[i] = count;

            /* update divisor for the next term */
            if (count == (seq_t)0)
                ddiv = 1;
            else
                ++ddiv;
        }
    }

    return seq[n];
}
