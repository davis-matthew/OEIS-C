/*
 * A288167  Fixed point of the mapping 00->0010, 1->000, starting with 00.
 * https://oeis.org/A288167
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

/* A288167: nth term of the fixed point of the substitution
   00 → 0010, 1 → 000 applied to the initial string "00".
   The function computes the n‑th binary digit (1‑based) by
   iteratively expanding until the position is generated. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 1‑based index, convert to 0‑based array position   */
    size_t target = (size_t)n;      /* n is guaranteed ≥ 1         */
    if (target < 2)                 /* handle n = 1 or 2 quickly  */
    {
        return (seq_t)0;
    }

    /*current string (bits stored as 0/1 in unsigned char) */
    size_t cur_len = 2;
    unsigned char *cur = (unsigned char*)malloc(cur_len);
    cur[0] = cur[1] = 0;            /* seed "00"                    */

    /* expand until we have at least target bits           */
    while (cur_len < target)
    {
        /* each symbol produces at most 4 output symbols */
        size_t next_cap = cur_len * 4;
        unsigned char *next = (unsigned char*)malloc(next_cap);
        size_t i = 0, next_len = 0;

        while (i < cur_len)
        {
            if (i + 1 < cur_len && cur[i] == 0 && cur[i + 1] == 0)
            {   /* 00 → 0010 */
                next[next_len++] = 0;
                next[next_len++] = 0;
                next[next_len++] = 1;
                next[next_len++] = 0;
                i += 2;
            }
            else if (cur[i] == 1)
            {   /* 1 → 000 */
                next[next_len++] = 0;
                next[next_len++] = 0;
                next[next_len++] = 0;
                ++i;
            }
            else
            {   /* single 0 that is not part of a pair stays 0 */
                next[next_len++] = 0;
                ++i;
            }
        }

        free(cur);
        cur = next;
        cur_len = next_len;
    }

    seq_t result = (seq_t)cur[target - 1];
    free(cur);
    return result;
}
