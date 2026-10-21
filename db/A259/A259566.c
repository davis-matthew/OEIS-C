/*
 * A259566  Numbers following gaps in the sequence of base-3 numbers that
 *          don't contain 0.
 * https://oeis.org/A259566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   base3_no_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A259566
   a(n) = the n‑th positive integer m such that
          m has no zero digit in its base‑3 representation,
          and m-1 contains at least one zero digit in base‑3.
   Equivalent to a(n) = A032924(2n‑1).  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/base3_no_zero.h"

/* Static cache storing computed terms of the sequence.  */
static seq_t *cache = NULL;       /* array of computed a(n) values   */
static size_t cache_len = 0;      /* number of stored terms          */
static size_t cache_cap = 0;      /* capacity of the array           */
static idx_t  last_i = 0;         /* last integer examined          */
static bool    last_flag = false; /* base3_no_zero(last_i)          */

/* Return the nth term of the sequence.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n = 0; safeguard */

    /* Allocate cache on first use.  */
    if (cache == NULL)
    {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return 0;            /* allocation failure – unlikely      */
        cache_len = 0;
        last_i = 0;
        last_flag = false;
    }

    /* Generate terms until we have at least n of them.  */
    while (cache_len < n)
    {
        ++last_i;
        bool cur = base3_no_zero(last_i);
        if (cur && !last_flag)
        {
            /* Grow cache if necessary.  */
            if (cache_len + 1 > cache_cap)
            {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL)
                    return 0;      /* allocation failure – unlikely      */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_i;
        }
        last_flag = cur;
    }

    return cache[n - 1];
}
