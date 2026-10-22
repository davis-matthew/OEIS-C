/*
 * A357677  Powers of either 3 or 5 or 7 (and 0).
 * https://oeis.org/A357677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: sorted union of powers of 3, 5, 7 and 0. 
   a(1)=0, a(2)=1, a(3)=3, a(4)=5, a(5)=7, a(6)=9, a(7)=25, ...
   The algorithm generates the terms in increasing order by a
   three‑way merge of the three geometric progressions. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache holding all already computed terms.
       cache[0] corresponds to a(1). */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;  /* number of terms stored */
    static size_t cap = 0;         /* allocated capacity */

    /* Current last powers used for each base.  They are kept
       across calls to generate further terms. */
    static seq_t pow3 = 1, pow5 = 1, pow7 = 1;

    /* Ensure cache is at least two terms (0 and 1). */
    if (cache == NULL) {
        size_t init_cap = 16;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failure */
        cache[0] = 0;
        cache[1] = 1;
        cached_len = 2;
        cap = init_cap;
    }

    /* Generate more terms until we have at least n of them. */
    while (cached_len < n) {
        /* Compute the next powers of 3, 5, and 7. */
        seq_t next3 = pow3 * (seq_t)3;
        seq_t next5 = pow5 * (seq_t)5;
        seq_t next7 = pow7 * (seq_t)7;

        /* Pick the smallest candidate. */
        seq_t next = next3;
        if (next5 < next) next = next5;
        if (next7 < next) next = next7;

        /* Grow cache if necessary. */
        if (cached_len == cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failure */
            cache = tmp;
            cap = new_cap;
        }

        /* Append the chosen value. */
        cache[cached_len++] = next;

        /* Advance the base(s) that contributed the next value. */
        if (next == next3) pow3 = next;
        if (next == next5) pow5 = next;
        if (next == next7) pow7 = next;
    }

    /* Return the requested term (indices are 1‑based). */
    return cache[n - 1];
}
