/*
 * A284471  1-limiting word of the morphism 0->1, 1->0100.
 * https://oeis.org/A284471
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

/* 1-limiting word of morphism 0→1, 1→0100.
   The word is obtained by repeatedly applying the composed morphism
   0 → 0100 (4 symbols) and 1 → 1010011 (7 symbols) starting from the
   single symbol '1'.  The n‑th term a(n) is the n‑th symbol of the
   resulting limit word.  For any n ≥ 1 the value can be found by
   expanding the current word until its length is at least n and then
   returning the (n – 1)-th element. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* current word */
    static idx_t  clen  = 0;      /* current length */

    /* initialise with the starting symbol '1' */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;
        clen = 1;
    }

    /* expand until the word is long enough to answer the query */
    while (clen < n) {
        /* compute length of the next word */
        idx_t next_len = 0;
        for (idx_t i = 0; i < clen; ++i) {
            next_len += (cache[i] == (seq_t)0) ? 4 : 7;
        }

        /* build the expanded word */
        seq_t *next = (seq_t *)malloc(next_len * sizeof(seq_t));
        idx_t pos = 0;
        for (idx_t i = 0; i < clen; ++i) {
            if (cache[i] == (seq_t)0) {
                /* 0 → 0100 */
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)0;
            } else {
                /* 1 → 1010011 */
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)1;
            }
        }

        /* replace old word with the new one */
        free(cache);
        cache = next;
        clen  = next_len;
    }

    return cache[(size_t)(n - 1)];
}
