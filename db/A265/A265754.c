/*
 * A265754  Reduced frequency counts for A004001: a(n) = A265332(n+1) -
 *          A036987(n).
 * https://oeis.org/A265754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sequence obtained by concatenating the blocks R_n, where
 *            R_1 = (1) and each subsequent R_{n} is formed by replacing every
 *            term k of R_{n-1} with the block 1,2,…,k and then appending n.
 *            The values produced are a(1), a(2), … . This sequence is
 *            identical to a(n)=A265332(n+1)-A036987(n)
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Returns the n-th term of OEIS A265754 (1‑based index).  */
seq_t a(idx_t n)
{
    if (n == 0)                  /* undefined, return 0 for safety */
        return (seq_t)0;

    /* Internal static cache of the sequence.  */
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;   /* capacity of the seq array (ignoring index 0) */
    static size_t seq_len = 0;   /* number of terms currently stored            */
    static size_t level = 0;     /* highest fully generated level               */

    /* Initialise cache on first use.  */
    if (!seq) {
        seq_cap = 16;
        seq = (seq_t *)malloc((seq_cap + 1) * sizeof(seq_t)); /* index 0 unused */
        seq_len = 0;
        level  = 0;
    }

    /* Ensure we have generated enough terms to answer the query. */
    if (seq_len < (size_t)n) {

        /* If nothing has been generated yet, start with R_1.  */
        if (seq_len == 0) {
            seq[1] = (seq_t)1;
            seq_len = 1;
            level = 1;          /* level 1 has been fully generated */
        }

        /* Generate successive levels until we reach at least n terms. */
        while (seq_len < (size_t)n) {

            /* Produce the next level (level + 1).  */
            level++;  /* new level number */

            /* Previous level starts at index 2^(level-2) and has length 2^(level-2).  */
            size_t prev_start = ((size_t)1 << (level - 2));   /* start index of level (level-1)  */
            size_t prev_len   = ((size_t)1 << (level - 2));   /* length of level (level-1)      */

            /* Number of terms that will be added for this level is 2^(level-1).  */
            size_t added_terms = ((size_t)1 << (level - 1));
            size_t needed_cap = seq_len + added_terms;

            /* Grow the cache if necessary.  */
            if (needed_cap > seq_cap) {
                size_t new_cap = seq_cap * 2;
                if (new_cap < needed_cap) new_cap = needed_cap;
                seq = (seq_t *)realloc(seq, (new_cap + 1) * sizeof(seq_t));
                seq_cap = new_cap;
            }

            /* Map each element of the previous level to a block 1..t.  */
            for (size_t i = 0; i < prev_len; ++i) {
                seq_t t = seq[prev_start + i];
                for (size_t v = 1; v <= (size_t)t; ++v) {
                    seq[++seq_len] = (seq_t)v;
                }
            }

            /* Append the final element of the current level.  */
            seq[++seq_len] = (seq_t)level;
        }
    }
    return seq[(size_t)n];
}
