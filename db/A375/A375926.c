/*
 * A375926  Numbers k such that A018252(k+1) = A018252(k) + 1. In other words,
 *          the k-th nonprime number is 1 less than the next.
 * https://oeis.org/A375926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that A018252(k+1) = A018252(k) + 1
   (i.e. two consecutive nonprime numbers, and k is the index of the
   first of the pair in the sequence of nonprime numbers). */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Static cache to hold computed terms of the sequence */
    static seq_t *cache = NULL;
    static size_t cache_len  = 0;  /* number of terms stored in cache      */
    static size_t cache_cap  = 0;  /* current capacity of the cache array  */

    /* State of the linear search over the positive integers */
    static idx_t last_checked        = 0;    /* last integer examined (1‑based)      */
    static idx_t nonprime_counter    = 0;    /* count of nonprime numbers seen      */
    static bool   prev_is_nonprime   = false;/* whether previous integer was nonprime*/
    static idx_t prev_nonprime_index = 0;    /* k of previous nonprime already seen*/
    static idx_t prev_value          = 0;    /* value of previous integer           */

    /* Generate terms until we have at least n terms */
    while (cache_len < n)
    {
        /* Advance to the next integer */
        ++last_checked;

        bool nonprime = !is_prime(last_checked);

        if (nonprime)
        {
            /* New nonprime found */
            ++nonprime_counter;

            /* If the previous integer was also a nonprime and they are
               consecutive, record the index of the previous nonprime
               (that is a term of the sequence). */
            if (prev_is_nonprime && last_checked == prev_value + 1)
            {
                if (cache_len == cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *new_cache =
                        (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!new_cache)
                        abort();  /* allocation failed */
                    cache = new_cache;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)prev_nonprime_index;
            }

            /* Update state for next iteration */
            prev_value              = last_checked;
            prev_is_nonprime        = true;
            prev_nonprime_index     = nonprime_counter;
        }
        else
        {
            /* Current integer is prime – just remember the flag */
            prev_is_nonprime = false;
            prev_value       = last_checked;
        }
    }

    /* n is 1‑based; cache indices are 0‑based */
    return cache[(size_t)(n - 1)];
}
