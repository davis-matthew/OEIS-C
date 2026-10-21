/*
 * A288306  Fixed point of the mapping 00->0010, 1->110, starting with 00.
 * https://oeis.org/A288306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A288306: a(n) = nth bit of the fixed point of the substitution
   00 → 0010, 1 → 110, starting with 00.  */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n < 1, but guard anyway */

    /* target index in 0‑based notation */
    idx_t target = n - 1;

    /* start with the word "00" */
    size_t cur_len = 2;
    unsigned char *cur = (unsigned char *)malloc(cur_len);
    if (!cur) return (seq_t)0;    /* allocation failure fallback */
    cur[0] = cur[1] = 0;

    /* grow the word until it contains the target position */
    while (cur_len <= (size_t)target)
    {
        /* allocate a generous temporary buffer; it will be trimmed later */
        size_t next_cap = cur_len * 4;
        unsigned char *next = (unsigned char *)malloc(next_cap);
        if (!next) { free(cur); return (seq_t)0; }

        size_t i = 0;       /* read index in current word */
        size_t ni = 0;      /* write index in new word */

        while (i < cur_len)
        {
            if (cur[i] == 1)
            {
                /* 1 → 110 */
                next[ni++] = 1;
                next[ni++] = 1;
                next[ni++] = 0;
                ++i;
            }
            else        /* cur[i] == 0 */
            {
                if (i + 1 < cur_len && cur[i + 1] == 0)
                {
                    /* 00 → 0010, consume the pair */
                    next[ni++] = 0;
                    next[ni++] = 0;
                    next[ni++] = 1;
                    next[ni++] = 0;
                    i += 2;
                }
                else
                {
                    /* single 0 stays 0 */
                    next[ni++] = 0;
                    ++i;
                }
            }
        }

        free(cur);
        cur     = next;
        cur_len = ni;           /* actual used length */
    }

    seq_t result = cur[target];
    free(cur);
    return result;
}
