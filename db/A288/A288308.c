/*
 * A288308  Positions of 1 in A288306; complement of A288307.
 * https://oeis.org/A288308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A288306 (fixed point of
 *            00->0010, 1->110 starting with 00); computed by expanding the
 *            word until it has n ones
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9418 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    size_t cur_len = 2;
    unsigned char *cur = (unsigned char *)malloc(cur_len);
    if (!cur) return (seq_t)0;
    cur[0] = cur[1] = 0;

    for (;;)
    {
        /* count ones in the current word; look for the n-th */
        idx_t cnt = 0;
        for (size_t k = 0; k < cur_len; ++k)
        {
            if (cur[k] == 1)
            {
                ++cnt;
                if (cnt == n)
                {
                    seq_t r = (seq_t)(k + 1);
                    free(cur);
                    return r;
                }
            }
        }

        size_t next_cap = cur_len * 4 + 4;
        unsigned char *next = (unsigned char *)malloc(next_cap);
        if (!next) { free(cur); return (seq_t)0; }

        size_t i = 0, ni = 0;
        while (i < cur_len)
        {
            if (cur[i] == 1)
            {
                next[ni++] = 1;
                next[ni++] = 1;
                next[ni++] = 0;
                ++i;
            }
            else if (i + 1 < cur_len && cur[i + 1] == 0)
            {
                next[ni++] = 0;
                next[ni++] = 0;
                next[ni++] = 1;
                next[ni++] = 0;
                i += 2;
            }
            else
            {
                next[ni++] = 0;
                ++i;
            }
        }

        free(cur);
        cur = next;
        cur_len = ni;
    }
}
