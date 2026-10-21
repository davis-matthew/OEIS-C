/*
 * A287106  Positions of 1 in A287104.
 * https://oeis.org/A287106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Positions of 1 in A287104.  The sequence a(n) is produced by
   iteratively applying the morphism
       0 → 10,
       1 → 12,
       2 → 0,
   to the initial symbol '1' and scanning the growing word
   for the nth symbol '1'. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)            /* sequence is defined for n ≥ 1 */
        return (seq_t)0;

    /* initial word: 1 */
    uint8_t *cur = (uint8_t *)malloc(1);
    cur[0] = 1;
    idx_t len_cur = 1;    /* length of current word */
    idx_t ones_cur = 1;   /* number of 1‑symbols already seen (the seed) */

    while (1) {
        /* compute length of next word after one substitution step */
        idx_t len_next = 0;
        for (idx_t i = 0; i < len_cur; ++i) {
            uint8_t v = cur[i];
            if (v == 0 || v == 1)
                len_next += 2;
            else
                len_next += 1;
        }

        /* build the next word and count 1‑symbols while building */
        uint8_t *next = (uint8_t *)malloc(len_next);
        idx_t out = 0;          /* position in the new word (0‑based) */
        idx_t ones_next = 0;    /* total number of 1‑symbols in the new word */

        for (idx_t i = 0; i < len_cur; ++i) {
            uint8_t v = cur[i];
            if (v == 0) {            /* 0 → 10 */
                next[out++] = 1;
                if (++ones_next == n) {
                    free(cur);
                    free(next);
                    return (seq_t)(out);   /* positions are 1‑based */
                }
                next[out++] = 0;
            } else if (v == 1) {     /* 1 → 12 */
                next[out++] = 1;
                if (++ones_next == n) {
                    free(cur);
                    free(next);
                    return (seq_t)(out);
                }
                next[out++] = 2;
            } else {                 /* 2 → 0 */
                next[out++] = 0;
            }
        }

        /* if the nth 1 has not appeared yet, continue with the new word */
        if (ones_next < n) {
            free(cur);
            cur = next;
            len_cur = len_next;
            ones_cur = ones_next;
            continue;
        }

        /* The nth 1 lies in the new word but was not found during
           the incremental build (this happens only when n is larger
           than any 1 in the whole word, which is impossible here). */
        idx_t seen = 0;
        for (idx_t i = 0; i < len_next; ++i) {
            if (next[i] == 1) {
                ++seen;
                if (seen == n) {
                    free(cur);
                    free(next);
                    return (seq_t)(i + 1);
                }
            }
        }

        /* In theory we should never reach this point */
        free(cur);
        free(next);
        return (seq_t)0;
    }
}
