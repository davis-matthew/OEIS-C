/*
 * A338090  Numbers having at least one 8 in their representation in base 9.
 * https://oeis.org/A338090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = smallest integer >0 whose base‑9 representation contains
 *            at least one digit 8, ranked in increasing order
 * Range:     u8 n <= 52, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle cases below the offset (n < 1).  The OEIS specification states that
       behaviour for such n is irrelevant.  We return 0 for convenience. */
    if (n < 1) return 0;

    /* Static cache of computed terms.
       cache[0] holds a(1), cache[1] holds a(2), etc.  */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* current allocation size */
    static size_t count = 0;   /* number of terms stored */
    static seq_t next_candidate = 1; /* next natural number to test */

    /* Grow the result to include term n if necessary. */
    while (count < (size_t)n)
    {
        /* Find the next integer containing at least one '8' in base 9. */
        seq_t val = next_candidate;
        while (1)
        {
            seq_t t = val;
            bool has8 = false;
            /* Examine each digit of val in base 9. */
            while (t > 0)
            {
                if (t % 9 == 8)
                {
                    has8 = true;
                    break;
                }
                t /= 9;
            }
            if (has8)
                break;                 /* val is the next desired term   */
            ++val;                     /* otherwise try the next number   */
        }

        /* Store the found value. */
        if (count >= cap)
        {
            size_t newcap = (cap == 0) ? 1024 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) /* allocation failure – abort gracefully */
                exit(1);
            cache = tmp;
            cap = newcap;
        }
        cache[count++] = val;

        /* Advance the candidate for the next invocation. */
        next_candidate = val + 1;
    }

    /* Return the requested term.  Cached terms are 1‑indexed. */
    return cache[n - 1];
}
