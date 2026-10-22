/*
 * A347290  Arnoux-Rauzy word sigma_0 x sigma_2 x sigma_1. Fixed point of the
 *          morphism 0-> 0201020, 1->1020, 2->201020 starting from a(1)=0.
 * https://oeis.org/A347290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth symbol of the fixed point of the morphism 0 →
 *            0201020, 1 → 1020, 2 → 201020, starting from the letter 0
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, but safe fallback */

    /* initial word: "0" */
    size_t cur_len = 1;
    seq_t *cur = (seq_t *)malloc(cur_len * sizeof(seq_t));
    cur[0] = (seq_t)0;

    /* iterate until the word reach at least n symbols */
    while (cur_len < (size_t)n)
    {
        size_t next_len = 0;
        for (size_t i = 0; i < cur_len; ++i)
        {
            seq_t c = cur[i];
            if (c == 0)      next_len += 7;   /* 0201020 */
            else if (c == 1) next_len += 4;   /* 1020    */
            else            next_len += 6;   /* 201020   */
        }

        seq_t *next = (seq_t *)malloc(next_len * sizeof(seq_t));
        size_t pos = 0;
        for (size_t i = 0; i < cur_len; ++i)
        {
            seq_t c = cur[i];
            if (c == 0)            /* 0 → 0201020 */
            {
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)2;
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)2;
                next[pos++] = (seq_t)0;
            }
            else if (c == 1)       /* 1 → 1020   */
            {
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)2;
                next[pos++] = (seq_t)0;
            }
            else                   /* 2 → 201020 */
            {
                next[pos++] = (seq_t)2;
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)1;
                next[pos++] = (seq_t)0;
                next[pos++] = (seq_t)2;
                next[pos++] = (seq_t)0;
            }
        }

        free(cur);
        cur      = next;
        cur_len  = next_len;
    }

    seq_t result = cur[n - 1];
    free(cur);
    return result;
}
