/*
 * A288752  0-limiting word of the mapping 00->0110, 1->01, starting with 00.
 * https://oeis.org/A288752
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

/* Sequence A288752:
 *   a(n) is the nth (1‑based) symbol of the infinite word generated
 *   by the context‑sensitive rewriting rule
 *     00 → 0110   and   1 → 01,
 *   starting from the word “00”.  The limit word is obtained by
 *   iterating the rule and taking the 0‑limiting sequence – the limit
 *   of the words of odd index.  The function returns the symbol as
 *   an integer 0 or 1. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* safety for invalid indices */

    /* Convert to a 0‑based index for internal use */
    size_t idx = (size_t)(n - 1);

    /* First odd iteration: word “0110” corresponding to iteration 1 */
    size_t cur_len = 4;
    char *cur = (char *)malloc(cur_len);
    if (!cur) return (seq_t)0;
    cur[0] = '0'; cur[1] = '1'; cur[2] = '1'; cur[3] = '0';

    size_t iter = 1;                           /* current iteration number */

    /* Generate successive words until an odd iteration has length ≥ n */
    while (cur_len < idx + 1 || (cur_len >= idx + 1 && (iter % 2) == 0))
    {
        /* Allocate enough space: each symbol expands to at most 4 chars */
        size_t new_cap = cur_len * 4 + 4;
        char *next = (char *)malloc(new_cap);
        if (!next) { free(cur); return (seq_t)0; }

        size_t j = 0;
        for (size_t i = 0; i < cur_len; )
        {
            if (cur[i] == '0' && (i + 1) < cur_len && cur[i + 1] == '0')
            {
                next[j++] = '0'; next[j++] = '1'; next[j++] = '1'; next[j++] = '0';
                i += 2;
            }
            else if (cur[i] == '0')
            {
                next[j++] = '0';
                ++i;
            }
            else   /* cur[i] == '1' */
            {
                next[j++] = '0'; next[j++] = '1';
                ++i;
            }
        }
        size_t next_len = j;

        free(cur);
        cur = next;
        cur_len = next_len;
        ++iter;
    }

    seq_t value = (seq_t)(cur[idx] - '0');
    free(cur);
    return value;
}
