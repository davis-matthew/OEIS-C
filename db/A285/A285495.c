/*
 * A285495  0-limiting word of the morphism 0->11, 1-> 0010.
 * https://oeis.org/A285495
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

/* Computes nth term of A285495: 0‑limiting word of the morphism
   0→11, 1→0010.  The algorithm iterates the morphism until an
   even number of steps has produced a word whose length is at least
   n, then returns the (n‑1)st symbol (0‑based). */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1, return 0 */

    size_t target = (size_t)n;       /* target prefix length */
    unsigned char *cur = (unsigned char *)malloc(sizeof(unsigned char));
    cur[0] = 0;                      /* start word: "0" */
    size_t cur_len = 1;
    idx_t iter = 0;                  /* 0 is even */

    /* Expand until we have an even number of iterations and the word
       is long enough to contain the n‑th symbol. */
    while (cur_len < target || (iter & 1)) {
        unsigned char *next = (unsigned char *)malloc(target * sizeof(unsigned char));
        size_t pos = 0;
        for (size_t i = 0; i < cur_len && pos < target; ++i) {
            if (cur[i] == 0) {           /* 0 → 1 1 */
                next[pos++] = 1;
                if (pos < target) next[pos++] = 1;
            } else {                     /* 1 → 0 0 1 0 */
                next[pos++] = 0;
                if (pos < target) next[pos++] = 0;
                if (pos < target) next[pos++] = 1;
                if (pos < target) next[pos++] = 0;
            }
        }
        free(cur);
        cur = next;
        cur_len = pos;
        ++iter;
    }

    seq_t result = (seq_t)(cur[target - 1] ? 1 : 0);
    free(cur);
    return result;
}
