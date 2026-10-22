/*
 * A341170  Numbers that when divided by the sum of their digits leave 3 as
 *          remainder.
 * https://oeis.org/A341170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 4542, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A341170 – numbers n such that n % digit_sum(n) == 3 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;          /* cache of found terms                      */
    static size_t len = 0, cap = 0;    /* current length and capacity               */
    static seq_t cur = 1;              /* next integer to test (global, persistent) */

    if (n == 0) return 0;             /* offset is 1 – undefined for n = 0 */

    /* Generate terms until we have the nth one */
    while (len < n)
    {
        seq_t dsum = digit_sum(cur);
        if (dsum != 0 && cur % dsum == 3)
        {
            /* Grow buffer if needed */
            if (len == cap)
            {
                size_t newcap = (cap == 0) ? 8 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
                if (!tmp)
                {
                    free(buf);
                    buf = NULL;
                    cap = len = 0;
                    return 0;        /* allocation failed – safe default          */
                }
                buf = tmp;
                cap = newcap;
            }
            buf[len++] = cur;          /* store found term */
        }
        ++cur;                         /* advance to next integer */
    }

    return buf[n - 1];                  /* zero‑based index in the cache            */
}
