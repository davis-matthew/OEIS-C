/*
 * A284912  Fixed point of the morphism 0 -> 01, 1 -> 1010.
 * https://oeis.org/A284912
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

/* Computation: a(n) is the nth term of the fixed point of the morphism
 * 0 -> 01, 1 -> 1010 (OEIS A284912).  The function expands the word
 * until it contains the desired index and returns the corresponding bit. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* behaviour below the offset does not matter */
        return (seq_t)0;
    /* start with the initial word "0" */
    char *cur = (char *)malloc(1);
    if (!cur)
        return (seq_t)0;
    cur[0] = 0;
    idx_t len_cur = 1;

    /* keep expanding until the word is long enough */
    while (len_cur < n) {
        /* compute length of the next word */
        idx_t len_next = 0;
        for (idx_t i = 0; i < len_cur; ++i) {
            if (cur[i] == 0)
                len_next += 2;   /* 0 -> 01 */
            else
                len_next += 4;   /* 1 -> 1010 */
        }
        char *next = (char *)malloc(len_next);
        if (!next) {
            free(cur);
            return (seq_t)0;
        }
        /* generate the next word */
        idx_t pos = 0;
        for (idx_t i = 0; i < len_cur; ++i) {
            if (cur[i] == 0) {
                next[pos++] = 0;
                next[pos++] = 1;
            } else {
                next[pos++] = 1;
                next[pos++] = 0;
                next[pos++] = 1;
                next[pos++] = 0;
            }
        }
        free(cur);
        cur = next;
        len_cur = len_next;
    }

    /* extract the requested bit (0-based indexing) */
    seq_t val = (seq_t)(cur[n - 1] ? 1 : 0);
    free(cur);
    return val;
}
