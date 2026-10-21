/*
 * A287125  Fixed point of the mapping 00->001, 1->110, starting with 00.
 * https://oeis.org/A287125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth bit of the fixed point of the morphism 00 → 001, 1 →
 *            110 starting from 00. This bit is obtained by iteratively
 *            applying the substitution until the word length reaches n; the
 *            earlier part of the word never changes
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* the sequence is defined only for n ≥ 1, but we guard n == 0 */
    if (n == 0) return (seq_t)0;

    /* start with the seed word "00" */
    unsigned char *curr = (unsigned char *)malloc(sizeof(unsigned char) * 2);
    curr[0] = 0;
    curr[1] = 0;
    idx_t curr_len = 2;

    /* iterate until the current word is at least n long */
    while (curr_len < n) {
        idx_t next_len = 0;
        /* the maximum possible length after expansion is 3*curr_len */
        unsigned char *next = (unsigned char *)malloc(sizeof(unsigned char) * (3 * curr_len));

        for (idx_t i = 0; i < curr_len; ++i) {
            unsigned char c = curr[i];
            if (c == 0 && i + 1 < curr_len && curr[i + 1] == 0) {
                /* 00 → 001 */
                next[next_len++] = 0;
                next[next_len++] = 0;
                next[next_len++] = 1;
                ++i; /* skip the second zero of the pair */
            } else if (c == 1) {
                /* 1 → 110 */
                next[next_len++] = 1;
                next[next_len++] = 1;
                next[next_len++] = 0;
            } else { /* single 0 not part of "00" */
                next[next_len++] = 0;
            }
        }

        free(curr);
        curr = next;
        curr_len = next_len;
    }

    /* at this point curr[n-1] is the fixed-point bit */
    seq_t result = (seq_t)curr[n - 1];
    free(curr);
    return result;
}
