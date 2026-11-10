/*
 * A288754  Positions of 1 in A288752; complement of A288753.
 * https://oeis.org/A288754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A288752 (0-limiting word of
 *            00->0110, 1->01); computed by iterating the rewriting and
 *            scanning
 * Range:     u8 n <= 116, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 7941 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    size_t cur_len = 4;
    char *cur = (char *)malloc(cur_len);
    if (!cur) return (seq_t)0;
    cur[0] = 0; cur[1] = 1; cur[2] = 1; cur[3] = 0;
    size_t iter = 1;
    for (;;)
    {
        if (iter % 2 == 1)
        {
            idx_t cnt = 0;
            for (size_t i = 0; i < cur_len; i++)
            {
                if (cur[i] == 1)
                {
                    cnt++;
                    if (cnt == n)
                    {
                        free(cur);
                        return (seq_t)(i + 1);
                    }
                }
            }
        }
        size_t new_cap = cur_len * 4 + 4;
        char *next = (char *)malloc(new_cap);
        if (!next) { free(cur); return (seq_t)0; }
        size_t j = 0;
        for (size_t i = 0; i < cur_len; )
        {
            if (cur[i] == 0 && (i + 1) < cur_len && cur[i + 1] == 0)
            {
                next[j++] = 0; next[j++] = 1; next[j++] = 1; next[j++] = 0;
                i += 2;
            }
            else if (cur[i] == 0)
            {
                next[j++] = 0;
                ++i;
            }
            else
            {
                next[j++] = 0; next[j++] = 1;
                ++i;
            }
        }
        free(cur);
        cur = next;
        cur_len = j;
        ++iter;
    }
}
