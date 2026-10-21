/*
 * A245289  Balanced squarefrees (of order one): squarefree numbers which are
 *          the average of the previous squarefree number and the following
 *          squarefree number.
 * https://oeis.org/A245289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Balanced squarefrees (sequence A245289).
   The function scans the list of squarefree numbers in order,
   keeps the last two seen, and whenever three consecutive
   squarefree numbers form an arithmetic progression it stores
   the middle one.  The results are cached so that any subsequent
   call is answered in O(1) time. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* Edge case: OEIS offset is 1; for n==0 we return 0 */
    if (n == 0)
        return 0;

    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* state of the scanning process */
    static seq_t prev2 = 0, prev1 = 0;
    static int stage = 0;          /* 0 = none seen, 1 = one seen, 2 = two seen */
    static idx_t current = 1;      /* next integer to test for squarefree */

    /* Keep extending the cache until we have at least n terms */
    while (cache_len < n)
    {
        seq_t v;

        /* Find the next squarefree integer */
        do
        {
            v = (seq_t)current++;
        } while (!is_squarefree(v));

        /* Handle the first two squarefree numbers separately */
        if (stage == 0)            /* first squarefree */
        {
            prev2 = v;
            stage = 1;
            continue;
        }
        if (stage == 1)            /* second squarefree */
        {
            prev1 = v;
            stage = 2;
            continue;
        }

        /* We now have a triple: prev2, prev1, v */
        seq_t d1 = prev1 - prev2;
        seq_t d2 = v - prev1;

        if (d1 == d2)
        {
            /* Ensure capacity */
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();  /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = prev1;   /* store the balanced number */
        }

        /* Shift for the next iteration */
        prev2 = prev1;
        prev1 = v;
    }

    /* Return the requested term (1‑based index) */
    return cache[n - 1];
}
