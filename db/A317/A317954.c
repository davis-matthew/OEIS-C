/*
 * A317954  Fixed point of the morphism 1 -> 12, 2 -> 13, 3 -> 14, 4 -> 1,
 *          starting from a(1) = 1.
 * https://oeis.org/A317954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed point of morphism 1→12, 2→13, 3→14, 4→1.
   The term a(n) (n≥1) is the nth element of this infinite word.
   The sequence is produced by iteratively expanding the current word
   until the required index is available. This method requires only
   O(n) time (≈10 expansions for n≈10 000) and is fully memoised for
   any order of calls. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t sz   = 0;

    /* initialise with the seed word "1" */
    if (sz == 0)
    {
        seq = (seq_t*)malloc(sizeof(seq_t));
        if (!seq) return 0;               /* unlikely, but safe */
        seq[0] = (seq_t)1;
        sz    = 1;
    }

    /* grow the word until it covers the requested index */
    while (sz < (size_t)n)
    {
        /* first pass: determine length of next word */
        size_t new_sz = sz;                /* each symbol keeps at least one */
        for (size_t i = 0; i < sz; ++i)
            if (seq[i] != (seq_t)4)        /* 1,2,3 map to two symbols */
                ++new_sz;

        /* allocate buffer for the expanded word */
        seq_t *new_seq = (seq_t*)malloc(new_sz * sizeof(seq_t));
        if (!new_seq) return 0;            /* unlikely, but safe */

        /* second pass: perform the expansion */
        size_t pos = 0;
        for (size_t i = 0; i < sz; ++i)
        {
            seq_t v = seq[i];
            if (v == (seq_t)1) { new_seq[pos++] = (seq_t)1; new_seq[pos++] = (seq_t)2; }
            else if (v == (seq_t)2) { new_seq[pos++] = (seq_t)1; new_seq[pos++] = (seq_t)3; }
            else if (v == (seq_t)3) { new_seq[pos++] = (seq_t)1; new_seq[pos++] = (seq_t)4; }
            else /* v==4 */          { new_seq[pos++] = (seq_t)1; }
        }

        /* free old word and replace */
        free(seq);
        seq = new_seq;
        sz  = new_sz;
    }

    return seq[(size_t)n - 1];
}
