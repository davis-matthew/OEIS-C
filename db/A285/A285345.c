/*
 * A285345  Fixed point of the morphism 0 -> 10, 1 -> 1100.
 * https://oeis.org/A285345
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

/* Sequence A285345: the n‑th term (1‑based) of the fixed point of the
   morphism 0 → 10, 1 → 1100, starting with 1.
   The term is 0 or 1, hence the implementation uses a small
   auxiliary buffer of unsigned 8‑bit values and casts the result
   to seq_t at the end.  This works for any seq_t type (unsigned
   integer or arbitrary‑precision integer), and for any idx_t
   (unsigned 64‑bit).  The algorithm runs in O(n) time and uses
   O(n) memory, which is well below the limits for n ≤ 10 000. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For any other index
       we return 0 safely. */
    if (n < 1)
        return 0;

    /* Work with a size_t for array indexing safely. */
    const size_t N = (size_t)n;

    /* Allocate two temporary arrays of unsigned 8‑bit values.
       Only 0 or 1 are stored, so the storage requirement is minimal
       and is safe for all seq_t implementations. */
    uint8_t *buf1 = (uint8_t *)malloc(N);
    uint8_t *buf2 = (uint8_t *)malloc(N);
    if (!buf1 || !buf2) {
        /* Allocation failed; fall back to a default value. */
        free(buf1);
        free(buf2);
        return 0;
    }

    /* Current word and its length.  Start with the single symbol 1. */
    uint8_t *cur  = buf1;
    uint8_t *next = buf2;
    size_t cur_len = 1;
    cur[0] = 1;

    /* Expand the word iteratively until it contains at least N symbols. */
    while (cur_len < N) {
        size_t next_len = 0;
        for (size_t i = 0; i < cur_len && next_len < N; ++i) {
            uint8_t v = cur[i];
            if (v == 0) {                /* 0 → 10 */
                if (next_len + 2 <= N) {
                    next[next_len++] = 1;
                    next[next_len++] = 0;
                } else {
                    if (next_len < N) next[next_len++] = 1;
                }
            } else {                     /* 1 → 1100 */
                if (next_len + 4 <= N) {
                    next[next_len++] = 1;
                    next[next_len++] = 1;
                    next[next_len++] = 0;
                    next[next_len++] = 0;
                } else {
                    if (next_len < N) next[next_len++] = 1;
                    if (next_len < N) next[next_len++] = 1;
                    if (next_len < N) next[next_len++] = 0;
                    if (next_len < N) next[next_len++] = 0;
                }
            }
        }
        /* Swap the buffers for the next round. */
        uint8_t *tmp = cur;
        cur = next;
        next = tmp;
        cur_len = next_len;
    }

    /* The nth symbol (1‑based) is at position N‑1 in the current buffer. */
    seq_t result = (seq_t)cur[N - 1];

    /* Free the auxiliary buffers. */
    free(buf1);
    free(buf2);

    return result;
}
