/*
 * A287066  Start with 1 and repeatedly substitute 0->01, 1->12, 2->0.
 * https://oeis.org/A287066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Morphic sequence A287066:
   Start with "1" and repeatedly substitute 0→01, 1→12, 2→0.
   The function returns the term a(n) (1‑based index).  The
   sequence is built iteratively by expanding the current word
   until it contains the requested position.  The implementation
   keeps the whole word in a static buffer so that successive
   calls are fast. */

#include <stdlib.h>  /* malloc, free */

/* External types supplied by the build system */
extern seq_t a(idx_t);

/* The function itself */
seq_t a(idx_t n)
{
    /* Do nothing for nonpositive indices (undefined in OEIS). */
    if (n < (idx_t)1)
        return (seq_t)0;

    static seq_t *buffer = NULL;
    static idx_t   length = 0;   /* current length of the word */

    /* Initialise with the starting word "1" on first call. */
    if (length == 0) {
        buffer = (seq_t*)malloc(sizeof(seq_t));
        if (!buffer) return (seq_t)0; /* allocation failure, return 0 */
        buffer[0] = (seq_t)1;
        length   = 1;
    }

    /* If the requested position is already present, return it. */
    if (n <= length)
        return buffer[n - 1];

    /* Expand the word until it is long enough. */
    while (length < n) {
        idx_t old_len = length;
        /* New length ≤ 2 × old_len; allocate a bit extra. */
        size_t alloc_cap = (size_t)(old_len * 2 + 1);
        seq_t *new_buf = (seq_t*)malloc(alloc_cap * sizeof(seq_t));
        if (!new_buf) return (seq_t)0; /* allocation failure */

        idx_t new_len = 0;
        for (idx_t i = 0; i < old_len; ++i) {
            seq_t v = buffer[i];
            if (v == (seq_t)0) {
                new_buf[new_len++] = (seq_t)0;
                new_buf[new_len++] = (seq_t)1;
            } else if (v == (seq_t)1) {
                new_buf[new_len++] = (seq_t)1;
                new_buf[new_len++] = (seq_t)2;
            } else { /* v == 2 */
                new_buf[new_len++] = (seq_t)0;
            }
        }

        free(buffer);
        buffer = new_buf;
        length = new_len;
    }

    /* Now length ≥ n, so the element is available. */
    return buffer[n - 1];
}
