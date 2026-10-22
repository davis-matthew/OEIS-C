/*
 * A323116  Fixed point of the morphism 1->221, 2->2211.
 * https://oeis.org/A323116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth term of the fixed point of the morphism 1 ->
 *            221, 2 -> 2211. The sequence begins 2, 2, 1, 1, 2, 2, 1, 1, 2,
 *            2, 1, 2, 2,
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the n-th element of that sequence.  It expands
   the morphism lazily and keeps the computed prefix in static storage,
   so repeated calls are efficient.  */

#include <stdlib.h>

static seq_t *seq = NULL;        /* store computed prefix */
static size_t seq_len = 0;       /* length of seq */

/* public function with external linkage */
seq_t a(idx_t n)
{
    /* n is 1‑based index,  n >= 1 is assumed */
    while (seq_len < (size_t)n)
    {
        if (seq_len == 0) /* initialise with the image of the seed '1' */
        {
            seq = (seq_t *)malloc(3 * sizeof(seq_t));
            if (!seq) return (seq_t)0; /* allocation failure */
            seq[0] = (seq_t)2;
            seq[1] = (seq_t)2;
            seq[2] = (seq_t)1;
            seq_len = 3;
            continue;
        }

        /* expand the current sequence once using the morphism */
        size_t old_len = seq_len;
        /* worst‑case size after expansion: each symbol produces 4 symbols */
        size_t new_cap = old_len * 4;
        seq_t *new_seq = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_seq) return (seq_t)0; /* allocation failure */

        size_t pos = 0;
        for (size_t i = 0; i < old_len; ++i)
        {
            seq_t v = seq[i];
            if (v == (seq_t)1)          /* 1 -> 221 */
            {
                new_seq[pos++] = (seq_t)2;
                new_seq[pos++] = (seq_t)2;
                new_seq[pos++] = (seq_t)1;
            }
            else /* v == 2 -> 2211 */
            {
                new_seq[pos++] = (seq_t)2;
                new_seq[pos++] = (seq_t)2;
                new_seq[pos++] = (seq_t)1;
                new_seq[pos++] = (seq_t)1;
            }
        }

        free(seq);
        seq = new_seq;
        seq_len = pos;
    }

    return seq[(size_t)n - 1];
}
